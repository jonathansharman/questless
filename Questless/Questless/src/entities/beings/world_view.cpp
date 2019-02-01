//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/world_view.hpp"

#include "entities/beings/being.hpp"
#include "entities/objects/object.hpp"
#include "game.hpp"
#include "world/region.hpp"

#include "vecx/hex_space.hpp"

#include <set>

using namespace media;

namespace ql {
	world_view::world_view(being const& being)
	    : _region{*being.region}
	    , _origin{being.coords}
	    , _visual_range{being.stats.a.vision.max_range()} //
	{
		ql::region const& region = _region;

		// Find the set of coordinates of sections possibly visible to the being.
		std::set<region_section::point> section_coords_set;
		for (span q = -_visual_range; q <= _visual_range; ++q) {
			for (span r = -_visual_range; r <= _visual_range; ++r) {
				region_tile::vector const offset{q, r};
				if (offset.length() <= _visual_range) {
					if (section const* section = region.containing_section(being.coords + offset)) {
						section_coords_set.insert(section->coords());
					}
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (region_section::point section_coords : section_coords_set) {
			section_view section_view;
			section_view.coords = section_coords;

			for (span q = 0_span; q < section::diameter; ++q) {
				for (span r = 0_span; r < section::diameter; ++r) {
					auto const region_tile_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});
					section_view.tile_perceptions[q.value][r.value] = being.perception_of(region_tile_coords);
				}
			}

			_section_views.push_back(std::move(section_view));

			// Calculate being visibilities.
			for (ql::being const& other_being : region.section_at(section_coords)->beings) {
				if (other_being.id == being.id) {
					// Can always perceive self fully.
					_entity_views.emplace_back(other_being.id, perception::maximum_level);
				} else {
					region_tile::point other_coords = other_being.coords;
					if ((being.coords - other_coords).length() <= _visual_range) {
						section_tile::point other_section_coords = section::section_tile_coords(other_coords);
						perception::level tile_perception =
						    section_view.tile_perceptions[other_section_coords.q.value][other_section_coords.r.value];

						_entity_views.emplace_back(other_being.id, tile_perception);
					}
				}
			}

			// Calculate object visibilities.
			for (object const& object : region.section_at(section_coords)->objects) {
				region_tile::point other_coords = object.coords;
				if ((being.coords - other_coords).length() <= _visual_range) {
					section_tile::point other_section_coords = section::section_tile_coords(other_coords);
					perception::level tile_perception =
					    section_view.tile_perceptions[other_section_coords.q.value][other_section_coords.r.value];
					_entity_views.emplace_back(object.id, tile_perception);
				}
			}
		}
	}
}
