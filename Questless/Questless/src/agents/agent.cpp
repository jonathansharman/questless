//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"
#include "world/region.hpp"

namespace ql
{
	// Quick Time Events

	complete agent::get_shock_quality(region_tile::point /*target_coords*/, std::function<complete(double)> cont) const
	{
		return cont(1.0);
	}

	complete agent::incant(gatestone& /*gatestone*/, std::function<complete(uptr<magic::spell>)> cont) const
	{
		return cont(nullptr);
	}

	// Actions

	complete agent::idle(action::cont cont)
	{
		return query_magnitude(std::make_unique<magnitude_query_wait_time>(), 10.0, 0.0, std::nullopt,
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

	complete agent::idle(double duration)
	{
		being.busy_time += duration;
		return complete{};
	}

	complete agent::turn(region_tile::direction direction, action::cont cont)
	{
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

	complete agent::walk(region_tile::direction direction, action::cont cont)
	{
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

	complete agent::fly()
	{
		//! @todo This.
		return complete{};
	}
}
