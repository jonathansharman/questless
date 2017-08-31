//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/magic/Gatestone.h"

#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	std::vector<uptr<Action>> Gatestone::actions()
	{
		std::vector<uptr<Action>> actions;
		if (_cooldown == 0.0) {
			actions.push_back(Incant::make(*this));
		}
		if (mana < _capacity) {
			actions.push_back(Charge::make(*this));
		}
		actions.push_back(Drop::make(*this));
		actions.push_back(Throw::make(*this));
		return actions;
	}

	void Gatestone::update()
	{
		_cooldown -= 1.0;

		// The proportion of a gatestone's mana that is lost each unit time.
		constexpr double discharge_rate = 0.001;
		mana *= (1.0 - discharge_rate);
	}

	Complete Gatestone::Charge::perform(Being& charger, Action::cont_t cont)
	{
		// The amount of mana transferred to a gatestone per time unit, ignoring intellect.
		double constexpr base_mana_transfer_rate = 1.0;

		// Determines the effect of intellect on transfer time.
		double constexpr mana_transfer_intellect_factor = 100.0;

		// Transfer as much mana as possible up to the amount needed to fill this gatestone.
		double const mana_to_full = _gatestone.capacity() - _gatestone.mana;
		double const transferred_mana = mana_to_full > charger.mana
			? charger.mana
			: mana_to_full
			;

		// Compute time to transfer mana.
		double const transfer_time = base_mana_transfer_rate / (1.0 + mana_transfer_intellect_factor * charger.stats.intellect);

		// Deduct the mana from the charging being.
		charger.mana -= transferred_mana;

		// Give the mana to this gatestone after a delay.
		class CompleteCharge : public Action
		{
		public:
			CompleteCharge(Gatestone& gatestone, double transferred_mana)
				: _gatestone_id{gatestone.id}
				, _transferred_mana{transferred_mana}
			{}
			std::string name() const final { return "Charge"; }
			Complete perform(Being&, cont_t cont) final
			{
				if (auto gatestone = game().items.ptr_as<Gatestone>(_gatestone_id)) {
					// As long as the gatestone still exists and the charger completed the channelling,
					// we don't really care where the gatestone is or what shape it's in.
					gatestone->mana += _transferred_mana;
				}
				// Even if the gatestone has disappeared, we'll consider getting to this point a successful performance.
				return cont(Action::Result::success);
			}
		private:
			Id<Item> _gatestone_id;
			double _transferred_mana;
		};
		uptr<Action> complete_charge = std::make_unique<CompleteCharge>(_gatestone, transferred_mana);
		return charger.add_delayed_action(transfer_time, std::move(cont), std::move(complete_charge));
	}

	Complete Gatestone::Incant::perform(Being& incanter, Action::cont_t cont)
	{
		if (incanter.stats.mute) {
			// Can't incant if mute.
			return incanter.agent().send_message(std::make_unique<MessageIncantFailedMute>(), [cont] { return cont(Action::Result::aborted); });
		} else {
			return incanter.agent().incant(_gatestone, [this, &incanter, cont = std::move(cont)](uptr<spell::Spell> spell) {
				if (spell) {
					// Incanter successfully incanted a spell.
					double const required_mana = spell->required_gatestone_mana();
					if (_gatestone.mana < required_mana) {
						// Not enough gatestone mana.
						double const mana_deficit = required_mana - _gatestone.mana;
						return incanter.agent().send_message(std::make_unique<MessageIncantGatestoneNotEnoughMana>(mana_deficit), [cont] { return cont(Action::Result::aborted); });
					} else {
						// Spend gatestone mana and cast the spell after the incantation delay.
						_gatestone.mana -= required_mana;
						double const incant_time = spell->incant_time(incanter);
						return incanter.add_delayed_action(incant_time, std::move(cont), spell::Spell::cast(std::move(spell)));
					}
				} else {
					// Incanter failed to incant a spell.
					//! @todo Time penalty for a failed or aborted cast?
					//! @todo Send a message that incantation failed.
					return cont(Action::Result::success);
				}
			});
		}
	}
}
