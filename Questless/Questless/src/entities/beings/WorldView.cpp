//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/WorldView.h"

#include <set>
using std::set;
#include <limits.h>

#include "Game.h"
#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "units/HexCoords.h"

using namespace sdl;
using namespace units;

namespace questless
{
	WorldView::WorldView(Being const& being, bool find_bounds)
		: _region{*being.region}
		, _bounds{std::nullopt}
	{
		Region const& region = _region;
		Vision vision = being.stats.vision;
		int visual_range = vision.max_range();

		// Find the set of coordinates of sections plausibly visible to the being.
		set<RegionSectionCoords> section_coords_set;
		for (int q = -visual_range; q <= visual_range; ++q) {
			for (int r = -visual_range; r <= visual_range; ++r) {
				RegionTileCoords offset{q, r};
				if (offset.length() <= visual_range) {
					if (Section const* section = region.containing_section(being.coords + offset)) {
						section_coords_set.insert(section->coords());
					}
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (RegionSectionCoords section_coords : section_coords_set) {
			SectionView section_view;
			section_view.coords = section_coords;

			for (int q = 0; q < Section::diameter; ++q) {
				for (int r = 0; r < Section::diameter; ++r) {
					auto region_tile_coords = Section::region_tile_coords(section_coords, SectionTileCoords{q, r});

					Perception tile_perception = being.perception_of(region_tile_coords);

					if (find_bounds && tile_perception.level > 0.0) {
						// Update bounding rectangle.
						GamePoint tile_game_point = Layout::dflt().to_world(region_tile_coords);
						if (!_bounds) {
							_bounds = GameRect{tile_game_point.x, tile_game_point.y, 0.0, 0.0};
						} else {
							_bounds->extend(tile_game_point);
						}	
					}
					section_view.tile_perceptions[q][r] = tile_perception;
				}
			}

			_section_views.push_back(std::move(section_view));

			// Calculate being visibilities.
			for (Being const& other_being : region.section(section_coords)->beings) {
				if (other_being.id == being.id) {
					// Can always perceive self fully.
					_being_views.emplace_back(other_being.id, Perception::maximum());
				} else {
					RegionTileCoords other_coords = other_being.coords;
					if (other_coords.distance_to(being.coords) <= visual_range) {
						SectionTileCoords other_section_coords = Section::section_tile_coords(other_coords);
						Perception tile_perception = section_view.tile_perceptions[other_section_coords.q][other_section_coords.r];

						_being_views.emplace_back(other_being.id, tile_perception);
					}
				}
			}

			// Calculate object visibilities.
			for (Object const& object : region.section(section_coords)->objects) {
				RegionTileCoords other_coords = object.coords;
				if (other_coords.distance_to(being.coords) <= visual_range) {
					SectionTileCoords other_section_coords = Section::section_tile_coords(other_coords);
					Perception tile_perception = section_view.tile_perceptions[other_section_coords.q][other_section_coords.r];
					_object_views.emplace_back(object.id, tile_perception);
				}
			}
		}
		if (_bounds) {
			//! @todo Figure out the correct way to compute these adjustments.

			//Point size = Layout::dflt().size.to_point();
			//Point double_size = (2 * Layout::dflt().size).to_point();
			//_bounds->x -= size.x;
			//_bounds->y -= size.y;
			//_bounds->w += double_size.x;
			//_bounds->h += double_size.y;

			GameVector size = Layout::dflt().size;
			GameVector double_size = 2 * Layout::dflt().size;
			_bounds->x -= 31;
			_bounds->y -= 19;
			_bounds->w += 62;
			_bounds->h += 39;
		}
	}
}
