/**
* @file    Agent.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Agent implementation.
*/

#include "agents/Agent.h"
#include "entities/beings/Being.h"
#include "world/Region.h"

namespace questless
{
	Action::Complete Agent::idle(Action::cont_t cont)
	{
		return query_magnitude("Wait", "Enter wait time.", 10.0, 0.0, boost::none,
			[this, cont](boost::optional<double> duration) {
				if (duration) {
					being.busy_time += *duration;
					return cont(Action::Result::success);
				} else {
					return cont(Action::Result::aborted);
				}
			}
		);
	}

	Action::Complete Agent::idle(double duration)
	{
		being.busy_time += duration;
		return Action::Complete{};
	}

	Action::Complete Agent::walk(RegionTileCoords::Direction direction, Action::cont_t cont)
	{
		if (being.stats.agility > 0.0) {
			double agility_factor = 100.0 / being.stats.agility;
			if (being.direction == direction) {
				if (being.region->move(being, being.coords.neighbor(direction))) {
					being.busy_time += agility_factor; /// @todo Account for terrain.
					cont(Action::Result::success);
				} else {
					cont(Action::Result::aborted);
				}
			} else {
				/// @todo Account for terrain.
				being.busy_time += agility_factor * (0.05 + 0.05 * RegionTileCoords::distance(being.direction, direction));
				being.direction = direction;
			}
			return cont(Action::Result::success);
		} else {
			return cont(Action::Result::aborted);
		}
	}

	Action::Complete Agent::fly()
	{
		/// @todo This.
		return Action::Complete{};
	}
}
