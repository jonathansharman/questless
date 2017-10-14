//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/magic/gatestone.h"

#include "entities/beings/being.h"
#include "game.h"

namespace ql
{
	std::vector<uptr<action>> gatestone::actions()
	{
		std::vector<uptr<action>> actions;
		if (_cooldown == 0.0) {
			actions.push_back(incant::make(*this));
		}
		if (mana < _capacity) {
			actions.push_back(charge::make(*this));
		}
		actions.push_back(drop::make(*this));
		actions.push_back(toss::make(*this));
		return actions;
	}

	void gatestone::update()
	{
		_cooldown -= 1.0;

		// The proportion of a gatestone's mana that is lost each unit time.
		constexpr double discharge_rate = 0.001;
		mana *= (1.0 - discharge_rate);
	}

	complete gatestone::charge::perform(being& charger, action::cont cont)
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
		class complete_charge : public action
		{
		public:
			complete_charge(gatestone& gatestone, double transferred_mana)
				: _gatestone_id{gatestone.id}
				, _transferred_mana{transferred_mana}
			{}
			std::string name() const final { return "Charge"; }
			complete perform(being&, cont cont) final
			{
				if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
					// As long as the gatestone still exists and the charger completed the channelling,
					// we don't really care where the gatestone is or what shape it's in.
					gatestone->mana += _transferred_mana;
				}
				// Even if the gatestone has disappeared, we'll consider getting to this point a successful performance.
				return cont(action::result::success);
			}
		private:
			ql::id<item> _gatestone_id;
			double _transferred_mana;
		};
		return charger.add_delayed_action(transfer_time, std::move(cont), std::make_unique<complete_charge>(_gatestone, transferred_mana));
	}

	complete gatestone::incant::perform(being& incanter, action::cont cont)
	{
		if (incanter.stats.mute) {
			// Can't incant if mute.
			return incanter.agent().send_message(std::make_unique<message_incant_failed_mute>(), [cont] { return cont(action::result::aborted); });
		} else {
			return incanter.agent().incant(_gatestone, [this, &incanter, cont = std::move(cont)](uptr<magic::spell> spell) {
				if (spell) {
					// Incanter successfully incanted a spell.
					double const required_mana = spell->required_gatestone_mana();
					if (_gatestone.mana < required_mana) {
						// Not enough gatestone mana.
						double const mana_deficit = required_mana - _gatestone.mana;
						return incanter.agent().send_message(std::make_unique<message_incant_gatestone_not_enough_mana>(mana_deficit), [cont] { return cont(action::result::aborted); });
					} else {
						// Spend gatestone mana and cast the spell after the incantation delay.
						_gatestone.mana -= required_mana;
						double const incant_time = spell->incant_time(incanter);
						return incanter.add_delayed_action(incant_time, std::move(cont), magic::spell::cast(std::move(spell)));
					}
				} else {
					// Incanter failed to incant a spell.
					//! @todo Time penalty for a failed or aborted cast?
					//! @todo Send a message that incantation failed.
					return cont(action::result::success);
				}
			});
		}
	}
}
