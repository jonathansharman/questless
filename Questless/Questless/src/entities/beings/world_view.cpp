//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "entities/beings/world_view.hpp"

#include "entities/beings/being.hpp"
#include "world/hex_space.hpp"
#include "world/region.hpp"

#include <set>

namespace ql {
	world_view::world_view(id viewer_id)
		: center{reg.get<ql::location>(viewer_id)}
		, visual_range{max_visual_range(reg.get<body>(viewer_id).stats.a.vision_sources.cur)} //
	{
		auto& region = reg.get<ql::region>(center.region_id);

		// Iterate over the rhomboid specified by the location and visual range to find visible tiles and beings.
		for (pace q = -visual_range; q <= visual_range; ++q) {
			for (pace r = -visual_range; r <= visual_range; ++r) {
				auto const offset = tile_hex::vector{q, r};

				// Skip corner tiles that are out of range.
				if (offset.length() > visual_range) { continue; }
				auto const tile_coords = center.coords + offset;

				// Skip missing tiles.
				auto const o_tile_id = region.tile_id_at(tile_coords);
				if (!o_tile_id) { continue; }
				auto const tile_id = *o_tile_id;

				// Get tile perception and position.
				auto const tile_perception = perception_of(viewer_id, tile_coords);
				auto const tile_position = to_view_space(tile_coords);

				// Add tile view if perceptible.
				if (tile_perception > 0_perception) { tile_views.push_back({tile_id, tile_perception, tile_position}); }

				// Check for an entity on this tile.
				auto const o_other_id = region.entity_id_at(tile_coords);
				if (!o_other_id) { continue; }
				auto const other_id = *o_other_id;

				// Can always perceive self fully. Otherwise perception of entity matches perception of tile.
				auto const other_perception = other_id == viewer_id ? max_perception : tile_perception;

				// Add entity view if perceptible.
				if (other_perception > 0_perception) {
					entity_views.push_back({other_id, other_perception, tile_position});
				}
			}
		}
	}
}
