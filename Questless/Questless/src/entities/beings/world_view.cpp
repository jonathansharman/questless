//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/world_view.hpp"

#include "entities/beings/being.hpp"
#include "world/region.hpp"

#include "vecx/hex_space.hpp"

#include <set>

namespace ql {
	world_view::world_view(ent being_id)
		: center{reg.get<ql::location>(being_id)}
		, visual_range{max_visual_range(reg.get<body>(being_id).stats.a.vision_sources.cur)} //
	{
		auto& region = reg.get<ql::region>(center.region_id);

		// Find the set of coordinates of sections possibly visible to the being.
		std::set<region_section::point> section_coords_set;
		for (span q = -visual_range; q <= visual_range; ++q) {
			for (span r = -visual_range; r <= visual_range; ++r) {
				region_tile::vector const offset{q, r};
				if (offset.length() <= visual_range) {
					if (section const* section = region.containing_section(center.coords + offset)) {
						section_coords_set.insert(section->coords());
					}
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (region_section::point section_coords : section_coords_set) {
			section_view section_view;
			section_view.coords = section_coords;

			for (span q = 0_span; q < section_diameter; ++q) {
				for (span r = 0_span; r < section_diameter; ++r) {
					auto const region_tile_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});
					section_view.tile_perceptions[q.value][r.value] = perception_of(being_id, region_tile_coords);
				}
			}

			section_views.push_back(std::move(section_view));

			// Calculate entity visibilities.
			for (auto [other_coords, other_id] : region.section_at(section_coords)->entity_id_map) {
				if (other_id == being_id) {
					// Can always perceive self fully.
					entity_views.push_back({other_id, max_perception});
				} else {
					section_tile::point const other_section_coords = section::section_tile_coords(other_coords);
					perception tile_perception =
						section_view.tile_perceptions[other_section_coords.q.value][other_section_coords.r.value];

					entity_views.push_back({other_id, tile_perception});
				}
			}
		}
	}
}
