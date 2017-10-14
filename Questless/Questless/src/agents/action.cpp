//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/action.h"
#include "entities/beings/being.h"
#include "agents/agent.h"

using namespace std;

namespace ql
{
	function<bool(region_tile::point)> action::tile_in_range_predicate(being& actor, int range)
	{
		return [&actor, range](region_tile::point tile_coords) {
			return (tile_coords - actor.coords).length() <= range;
		};
	}

	function<bool(being&)> action::being_in_range_predicate(being& actor, int range)
	{
		return [&actor, range](being& being) {
			return (being.coords - actor.coords).length() <= range;
		};
	}
}
