//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agent.hpp"

#include "queries/all.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"
#include "magic/spell.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"

namespace ql {
	// Quick Time Events

	complete agent::aim_missile(region_tile::point /*source_coords*/,
		ql::being& target_being,
		std::function<complete(body_part*)> cont) {
		//! @todo There should be a miss chance based on distance and some internal skill stat. The agent should also
		//! prioritize certain parts depending on goals. Return a random body part for now.
		auto it = target_being.body.parts().begin();
		std::advance(it, uniform(std::size_t{0}, target_being.body.parts().size() - 1));
		return cont(&it->get());
	}

	complete agent::get_shock_quality(region_tile::point /*target_coords*/, std::function<complete(double)> cont) {
		return cont(1.0);
	}

	complete agent::incant(gatestone& /*gatestone*/, std::function<complete(std::optional<magic::spell> const&)> cont) {
		return cont(std::nullopt);
	}

	// Actions

	complete agent::idle(action::cont cont) {
		return query_count(queries::count::wait_time{}, 10, 0, std::nullopt, [this, cont](std::optional<int> duration) {
			if (duration) {
				this->being.cond.busy_time += tick{*duration}; //! @todo Ticks query?
				return cont(action::result::success);
			} else {
				return cont(action::result::aborted);
			}
		});
	}

	complete agent::idle(tick duration) {
		being.cond.busy_time += duration;
		return complete{};
	}

	complete agent::turn(region_tile::direction direction, action::cont cont) {
		if (being.stats.a.agility > 0.0_agi) {
			// Base cost of turning any amount.
			constexpr auto base_delay = 1_tick;

			// Cost per turning distance from current direction to new direction.
			constexpr auto delay_per_turn = 1_tick;

			auto const agility_factor = 100.0_agi / being.stats.a.agility.value();
			auto const turn_delay = delay_per_turn * region_tile::distance(being.cond.direction, direction);
			being.cond.busy_time += cancel::quantity_cast<tick>(agility_factor * (base_delay + turn_delay));
			being.cond.direction = direction;

			return cont(action::result::success);
		} else {
			return cont(action::result::aborted);
		}
	}

	complete agent::walk(region_tile::direction direction, action::cont cont) {
		if (being.stats.a.agility > 0.0_agi) {
			// Base cost of moving.
			constexpr auto base_delay = 1_tick;

			// Cost per turning distance from direction faced to direction moved.
			constexpr auto delay_per_turn = 1_tick;

			auto const agility_factor = 100.0_agi / being.stats.a.agility.value();
			if (being.location.region->try_move(being, being.location.coords.neighbor(direction))) {
				auto const strafe_delay = delay_per_turn * region_tile::distance(being.cond.direction, direction);
				//! @todo Account for terrain.
				being.cond.busy_time += cancel::quantity_cast<tick>(agility_factor * (base_delay + strafe_delay));

				return cont(action::result::success);
			}
		}
		return cont(action::result::aborted);
	}

	complete agent::fly() {
		//! @todo This.
		return complete{};
	}
}
