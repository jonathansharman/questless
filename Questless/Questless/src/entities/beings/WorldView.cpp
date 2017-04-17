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
		RegionTileCoords coords = being.coords;
		Vision vision = being.stats.vision;

		// Calculate the maximum distance the being can see.
		int visual_range = static_cast<int>(sqrt(vision.acuity / Vision::distance_factor));

		// Find the set of coordinates of sections plausibly visible to the being.
		set<RegionSectionCoords> section_coords_set;
		for (int q = -visual_range; q <= visual_range; ++q) {
			for (int r = -visual_range; r <= visual_range; ++r) {
				RegionTileCoords offset{q, r};
				if (offset.length() > visual_range) continue;
				if (Section const* section = region.containing_section(coords + offset)) {
					section_coords_set.insert(section->coords());
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

					bool in_front;
					auto offset = region_tile_coords - being.coords;
					switch (being.direction) {
						case RegionTileCoords::Direction::one:
							in_front = offset.q >= 0 && offset.q + offset.r >= 0;
							break;
						case RegionTileCoords::Direction::two:
							in_front = offset.r >= 0 && offset.q + offset.r >= 0;
							break;
						case RegionTileCoords::Direction::three:
							in_front = offset.q <= 0 && offset.r >= 0;
							break;
						case RegionTileCoords::Direction::four:
							in_front = offset.q <= 0 && offset.q + offset.r <= 0;
							break;
						case RegionTileCoords::Direction::five:
							in_front = offset.r <= 0 && offset.q + offset.r <= 0;
							break;
						case RegionTileCoords::Direction::six:
							in_front = offset.q >= 0 && offset.r <= 0;
							break;
						default:
							throw std::logic_error{"Invalid direction."};
					}
					if (in_front) {
						double light_level = region.tile(region_tile_coords)->light_level();
						double vision_divisor = 1.0 + (light_level - vision.ideal_light) * (light_level - vision.ideal_light) / vision.light_tolerance * Vision::light_factor;
						int distance = coords.distance_to(region_tile_coords);

						double tile_visibility = in_front ? (vision.acuity - distance * distance * Vision::distance_factor) / vision_divisor : 0.0;

						// Update bounding rectangle.
						if (find_bounds && tile_visibility > 0.0) {
							GamePoint tile_game_point = Layout::dflt().to_world(region_tile_coords);
							if (!_bounds) {
								_bounds = GameRect{tile_game_point.x, tile_game_point.y, 0.0, 0.0};
							} else {
								_bounds->extend(tile_game_point);
							}
						}
						section_view.tile_visibilities[q][r] = tile_visibility;
					} else {
						section_view.tile_visibilities[q][r] = 0.0;
					}
				}
			}

			_section_views.push_back(std::move(section_view));

			// Calculate being visibilities.
			for (Being const& other_being : region.section(section_coords)->beings()) {
				RegionTileCoords other_coords = other_being.coords;
				if (other_coords.distance_to(coords) < visual_range) {
					SectionTileCoords other_section_coords = Section::section_tile_coords(other_coords);
					double tile_visibility = section_view.tile_visibilities[other_section_coords.q][other_section_coords.r];
					
					if (tile_visibility >= _low_perception_threshold) {
						PerceptionLevel perception;
						if (tile_visibility < _medium_perception_threshold) {
							perception = PerceptionLevel::low;
						} else if (tile_visibility < _high_perception_threshold) {
							perception = PerceptionLevel::medium;
						} else if (tile_visibility < _full_perception_threshold) {
							perception = PerceptionLevel::high;
						} else {
							perception = PerceptionLevel::full;
						}
						_being_views.emplace_back(other_being.id, perception);
					}
				}
			}

			// Calculate object visibilities.
			for (Object const& object : region.section(section_coords)->objects()) {
				RegionTileCoords other_coords = object.coords;
				if (other_coords.distance_to(coords) < visual_range) {
					SectionTileCoords other_section_coords = Section::section_tile_coords(other_coords);
					double tile_visibility = section_view.tile_visibilities[other_section_coords.q][other_section_coords.r];

					if (tile_visibility >= _low_perception_threshold) {
						PerceptionLevel perception;
						if (tile_visibility < _medium_perception_threshold) {
							perception = PerceptionLevel::low;
						} else if (tile_visibility < _high_perception_threshold) {
							perception = PerceptionLevel::medium;
						} else if (tile_visibility < _full_perception_threshold) {
							perception = PerceptionLevel::high;
						} else {
							perception = PerceptionLevel::full;
						}
						_object_views.emplace_back(object.id, perception);
					}
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
