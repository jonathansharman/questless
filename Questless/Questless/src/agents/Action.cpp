//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/Action.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

using namespace std;

namespace questless
{
	function<bool(RegionTile::Point)> Action::tile_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](RegionTile::Point tile_coords) {
			return (tile_coords - actor.coords).length() <= range;
		};
	}

	function<bool(Being&)> Action::being_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](Being& being) {
			return (being.coords - actor.coords).length() <= range;
		};
	}
}
