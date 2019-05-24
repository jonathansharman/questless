//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "basic_ai.hpp"

#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "utility/random.hpp"
#include "utility/visitation.hpp"

namespace ql {
	action::result basic_ai::act() {
		return match(_state,
			[this](idle_state) {
				if (bernoulli_trial(0.1)) {
					// Walk next time.
					_state = walk_state{};
				}
				return action::result::success;
			},
			[this](walk_state) {
				auto const& cond = reg.get<conditions const>(id);
				// Randomly either move in current direction or turn towards a random direction.
				action::result const result = random_bool() ? walk(cond.direction) : turn(random_direction());
				if (result == action::result::success) {
					// Idle next time.
					_state = idle_state{};
				} else {
					// Couldn't walk or turn. Try again next time.
				}
				return result;
			},
			[this](attack_state const& as) {
				if (!reg.valid(as.target_id)) {
					// Target not found. Switch to idle state.
					_state = idle_state{};
					return action::result::failure;
				}
				auto [target, target_location, target_cond] = reg.get<being, location, conditions>(as.target_id);
				if (perception::get_category(target.perception_of(target_location.coords)) == perception::category::none) {
					// Target not visible. Switch to idle state.
					_state = idle_state{};
					return action::result::failure;
					//! @todo Only go passive while target is out of visual range. Keep a grudge list?
				} else {
					auto target_direction = (target_location.coords - target_location.coords).direction();
					if (target_cond.direction != target_direction) {
						// Facing away from target. Turn towards it.
						return turn(target_direction);
					} else {
						// Facing towards target.
						if ((target_location.coords - target_location.coords).length() == 1_span) {
							// Within striking distance of target.

							//! @todo Find and use melee weapon in inventory, if present.
							return action::result::failure;
						} else {
							// Out of range. Move towards target.
							return walk(target_direction);
						}
					}
				}
			});
	}

	void basic_ai::perceive(effects::effect const& effect) {
		if (std::holds_alternative<effects::injury>(effect.value)) {
			// Retaliate against injuries.
			effects::injury const& injury = std::get<effects::injury>(effect.value);
			if (injury.opt_source_id && injury.target_being_id == id) { _state = attack_state{*injury.opt_source_id}; }
		} else {
			// Ignore all other effects.
		}
	}
}
