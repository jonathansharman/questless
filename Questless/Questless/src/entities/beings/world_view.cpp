//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/world_view.hpp"

#include <set>
#include <limits.h>

#include "game.hpp"
#include "entities/beings/being.hpp"
#include "entities/objects/object.hpp"
#include "units/hex_space.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	world_view::world_view(being const& being, bool find_bounds)
		: _region{*being.region}
		, _origin{being.coords}
		, _visual_range{being.stats.a.vision.max_range()}
		, _bounds{std::nullopt}
	{
		ql::region const& region = _region;

		// Find the set of coordinates of sections plausibly visible to the being.
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
					auto region_tile_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});

					perception::level tile_perception = being.perception_of(region_tile_coords);

					if (find_bounds && tile_perception > 0.0_perception) {
						// Update bounding rectangle.
						game_space::point tile_game_point = to_world(region_tile_coords);
						if (!_bounds) {
							_bounds = game_space::box{game_space::point{tile_game_point.x(), tile_game_point.y()}, game_space::vector::zero()};
						} else {
							_bounds->extend(tile_game_point);
						}	
					}
					section_view.tile_perceptions[q.value][r.value] = tile_perception;
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
						perception::level tile_perception = section_view.tile_perceptions[other_section_coords.q.value][other_section_coords.r.value];

						_entity_views.emplace_back(other_being.id, tile_perception);
					}
				}
			}

			// Calculate object visibilities.
			for (object const& object : region.section_at(section_coords)->objects) {
				region_tile::point other_coords = object.coords;
				if ((being.coords - other_coords).length() <= _visual_range) {
					section_tile::point other_section_coords = section::section_tile_coords(other_coords);
					perception::level tile_perception = section_view.tile_perceptions[other_section_coords.q.value][other_section_coords.r.value];
					_entity_views.emplace_back(object.id, tile_perception);
				}
			}
		}
		if (_bounds) {
			//! @todo Figure out the correct way to compute these adjustments.

			//point size = size.to_point();
			//point double_size = (2 * size).to_point();
			//_bounds->x -= size.x;
			//_bounds->y -= size.y;
			//_bounds->w += double_size.x;
			//_bounds->h += double_size.y;

			game_space::vector size = hex_layout.size;
			game_space::vector double_size = 2 * hex_layout.size;
			left(*_bounds) -= 31.0;
			top(*_bounds) -= 19.0;
			width(*_bounds) += 62.0;
			height(*_bounds) += 39.0;
		}
	}
}
