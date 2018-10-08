//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/magic/gatestone.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "magic/spell.hpp"

namespace ql {
	std::vector<uptr<action>> gatestone::actions() {
		std::vector<uptr<action>> actions;
		if (equipped()) {
			if (_cooldown == 0_tick) {
				actions.push_back(incant::make(*this));
			}
			actions.push_back(unequip::make(*this));
			actions.push_back(drop::make(*this));
			actions.push_back(toss::make(*this));
		} else {
			actions.push_back(equip::make(*this));
		}
		return actions;
	}

	void gatestone::update(tick elapsed) {
		_cooldown -= elapsed;

		// Charge over time.
		constexpr auto recharge_per_tick = 1.0_mp / 1_tick;
		charge += recharge_per_tick * elapsed; //! @todo Should recharge rate be a fixed amount or a percentage of capacity?
	}

	complete gatestone::incant::perform(being& incanter, action::cont cont) {
		if (incanter.stats.a.mute.value) {
			// Can't incant if mute.
			return incanter.agent().send_message(queries::message::incant_failed_mute{}, [cont] { return cont(action::result::aborted); });
		}
		// Ensure the gatestone still exists and is equipped to the incanter.
		if (auto gatestone = the_game().items.ptr_as<ql::gatestone>(_gatestone_id)) {
			if (gatestone->equipped() && *gatestone->opt_bearer_id() == incanter.id) {
				return incanter.agent().incant(*gatestone, [this, &incanter, gatestone, cont = std::move(cont)](uptr<magic::spell> spell) {
					if (spell) {
						// Incanter successfully incanted a spell. Cast it after the incantation delay.
						tick const incant_time = spell->incant_time(incanter);
						return incanter.add_delayed_action(incant_time, std::move(cont), magic::spell::cast(std::move(spell), gatestone->id));
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
