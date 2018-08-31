//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agent.hpp"

#include "queries/all_queries.hpp"

#include "entities/beings/being.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"

namespace ql {
	// Quick Time Events
	
	complete agent::aim_missile(region_tile::point /*source_coords*/, ql::being& target_being, std::function<complete(body_part*)> cont) const {
		//! @todo There should be a miss chance based on distance and some internal skill stat. The agent should also prioritize certain parts depending on goals. Return a random body part for now.
		auto it = target_being.body.parts().begin();
		std::advance(it, uniform(std::size_t{0}, target_being.body.parts().size() - 1));
		return cont(&it->get());
	}

	complete agent::get_shock_quality(region_tile::point /*target_coords*/, std::function<complete(double)> cont) const {
		return cont(1.0);
	}

	complete agent::incant(gatestone& /*gatestone*/, std::function<complete(uptr<magic::spell>)> cont) const {
		return cont(nullptr);
	}

	// Actions

	complete agent::idle(action::cont cont) {
		return query_magnitude(umake<magnitude_query_wait_time>(), 10.0, 0.0, std::nullopt,
			[this, cont](std::optional<double> duration) {
				if (duration) {
					this->being.busy_time += *duration;
					return cont(action::result::success);
				} else {
					return cont(action::result::aborted);
				}
			}
		);
	}

	complete agent::idle(double duration) {
		being.busy_time += duration;
		return complete{};
	}

	complete agent::turn(region_tile::direction direction, action::cont cont) {
		if (being.stats.agility > 0.0) {
			// Base cost of turning any amount, as a percentage of the agility factor.
			constexpr double base_cost_factor = 0.1;
			
			// Cost per turning distance from current direction to new direction, as a percentage of the agility factor.
			constexpr double cost_factor_per_turn = 0.1;

			double const agility_factor = 100.0 / being.stats.agility;
			//! @todo Account for terrain.
			double const turn_cost_factor = cost_factor_per_turn * region_tile::distance(being.direction, direction);
			being.busy_time += agility_factor * (base_cost_factor + turn_cost_factor);
			being.direction = direction;

			return cont(action::result::success);
		} else {
			return cont(action::result::aborted);
		}
	}

	complete agent::walk(region_tile::direction direction, action::cont cont) {
		if (being.stats.agility > 0.0) {
			// Base cost of moving, as a percentage of the agility factor.
			constexpr double base_cost_factor = 1.0;

			// Cost per turning distance from direction faced to direction moved, as a percentage of the agility factor.
			constexpr double cost_factor_per_turn = 0.1;

			double const agility_factor = 100.0 / being.stats.agility;
			if (being.region->move(being, being.coords.neighbor(direction))) {
				double const strafe_cost_factor = cost_factor_per_turn * region_tile::distance(being.direction, direction);
				being.busy_time += agility_factor * (base_cost_factor + strafe_cost_factor); //! @todo Account for terrain.
				
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
