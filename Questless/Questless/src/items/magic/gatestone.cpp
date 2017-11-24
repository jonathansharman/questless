//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/magic/gatestone.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql
{
	std::vector<uptr<action>> gatestone::actions()
	{
		std::vector<uptr<action>> actions;
		if (_cooldown == 0.0) {
			actions.push_back(incant::make(*this));
		}
		actions.push_back(toggle_autocharge::make(*this, true));
		if (equipped()) {
			actions.push_back(unequip::make(*this));
		} else {
			actions.push_back(equip::make(*this));
		}
		actions.push_back(drop::make(*this));
		actions.push_back(toss::make(*this));
		return actions;
	}

	void gatestone::update()
	{
		_cooldown -= 1.0;

		if (equipped()) {
			// Charge over time, if auto-charge is enabled.
			if (_autocharge && equipped()) {
				ql::id<being> bearer_id = *opt_bearer_id();
				if (being* bearer = the_game().beings.ptr(bearer_id)) {
					// The maximum amout of mana a gatestone absorbs per unit time while equipped and set to auto-charge.
					constexpr double recharge_per_turn = 1.0;

					// The amount of mana actually transferred is capped by the amount needed and the amount available.
					double const mana_to_full = _capacity - mana;
					double const transferred_mana = std::min({recharge_per_turn, mana_to_full, bearer->mana.value()});

					// Deduct the mana from the charging being.
					bearer->mana -= transferred_mana;
					mana += transferred_mana;
				}
			}
		} else {
			// Discharge over time if not equipped.
			constexpr double discharge_per_turn = 1.0;
			mana -= discharge_per_turn;
		}
	}

	complete gatestone::toggle_autocharge::perform(being& toggler, action::cont cont)
	{
		// Ensure the gatestone still exists and is equipped to the toggler.
		if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
			if (gatestone->equipped() && *gatestone->opt_bearer_id() == toggler.id) {
				if (_incur_delay) {
					// Time required to complete the toggle.
					double constexpr delay = 1.0;

					// Finish toggling after a delay.
					return toggler.add_delayed_action(delay, std::move(cont), toggle_autocharge::make(*gatestone, false));
				} else {
					// Already incurred the delay time; finish toggling now.
					gatestone->_autocharge = !gatestone->_autocharge;
				}
			}
		}
		// This action never fails, regardless of whether the gatestone is actually toggled.
		return cont(action::result::success);
	}

	complete gatestone::incant::perform(being& incanter, action::cont cont)
	{
		if (incanter.stats.mute) {
			// Can't incant if mute.
			return incanter.agent().send_message(std::make_unique<message_incant_failed_mute>(), [cont] { return cont(action::result::aborted); });
		} else {
			// Ensure the gatestone still exists and is equipped to the incanter.
			if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
				if (gatestone->equipped() && *gatestone->opt_bearer_id() == incanter.id) {
					return incanter.agent().incant(*gatestone, [this, &incanter, gatestone, cont = std::move(cont)](uptr<magic::spell> spell) {
						if (spell) {
							// Incanter successfully incanted a spell.
							double const required_mana = spell->required_gatestone_mana();
							if (gatestone->mana < required_mana) {
								// Not enough gatestone mana.
								double const mana_deficit = required_mana - gatestone->mana;
								return incanter.agent().send_message(std::make_unique<message_incant_gatestone_not_enough_mana>(mana_deficit), [cont] { return cont(action::result::aborted); });
							} else {
								// Spend gatestone mana and cast the spell after the incantation delay.
								gatestone->mana -= required_mana;
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
			// Gatestone missing.
			//! @todo Message?
			return cont(action::result::success);
		}
	}
}
