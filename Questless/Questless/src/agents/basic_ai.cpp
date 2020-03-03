//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "basic_ai.hpp"

#include "actions.hpp"

#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "entities/perception.hpp"
#include "utility/future.hpp"
#include "utility/random.hpp"
#include "utility/visitation.hpp"

namespace ql {
	basic_ai::basic_ai(reg& reg, id id) : _reg{&reg}, _id{id} {}

	auto basic_ai::act() -> std::future<void> {
		return match(
			_state,
			[this](idle_state) {
				if (bernoulli_trial(0.1)) {
					// Walk next time.
					_state = walk_state{};
				}
				return make_ready_future();
			},
			[this](walk_state) {
				// Randomly either move in current direction or turn towards a random direction.
				if (coin_flip()) {
					walk(*_reg, _id, _reg->get<body>(_id).cond.direction);
				} else {
					turn(*_reg, _id, random_direction());
				}
				// Idle next time.
				_state = idle_state{};
				return make_ready_future();
			},
			[this](attack_state const& as) {
				if (!_reg->valid(as.target_id)) {
					// Target not found. Switch to idle state.
					_state = idle_state{};
					return make_ready_future();
				}
				auto const& target_location = _reg->get<location>(as.target_id);
				if (perception_of(*_reg, _id, target_location.coords) <= 0_perception) {
					// Target not visible. Switch to idle state.
					_state = idle_state{};
					return make_ready_future();
					//! @todo Only go passive while target is out of visual range. Keep a grudge list?
				} else {
					auto target_direction = (target_location.coords - target_location.coords).direction();
					auto const& target_body = _reg->get<body>(as.target_id);
					if (target_body.cond.direction != target_direction) {
						// Facing away from target. Turn towards it.
						turn(*_reg, _id, target_direction);
						return make_ready_future();
					} else {
						// Facing towards target.
						if ((target_location.coords - target_location.coords).length() == 1_pace) {
							// Within striking distance of target.

							//! @todo Find and use melee weapon in inventory, if present.
							return make_ready_future();
						} else {
							// Out of range. Move towards target.
							walk(*_reg, _id, target_direction);
							return make_ready_future();
						}
					}
				}
			});
	}

	auto basic_ai::perceive(effects::effect const& effect) -> void {
		if (std::holds_alternative<effects::injury>(effect.value)) {
			// Retaliate against injuries.
			effects::injury const& injury = std::get<effects::injury>(effect.value);
			if (injury.o_source_id && injury.target_being_id == _id) { _state = attack_state{*injury.o_source_id}; }
		} else {
			// Ignore all other effects.
		}
	}
}
