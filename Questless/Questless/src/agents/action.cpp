//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "action.hpp"

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"

using namespace std;

namespace ql {
	function<bool(region_tile::point)> action::tile_in_range_predicate(being& actor, span range) {
		return [&actor, range](
				   region_tile::point tile_coords) { return (tile_coords - actor.location.coords).length() <= range; };
	}

	function<bool(being&)> action::being_in_range_predicate(being& actor, span range) {
		return
			[&actor, range](being& being) { return (being.location.coords - actor.location.coords).length() <= range; };
	}
}
