/**
* @file    Action.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Action abstract base class.
*/

#include "entities/beings/Action.h"
#include "entities/beings/Being.h"
#include "entities/beings/Agent.h"

using namespace std;

namespace questless
{
	function<bool(HexCoords)> Action::tile_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](HexCoords tile_coords) {
			return actor.coords().distance_to(tile_coords) <= range;
		};
	}

	function<bool(Being&)> Action::being_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](Being& being) {
			return actor.coords().distance_to(being.coords()) <= range;
		};
	}
}
