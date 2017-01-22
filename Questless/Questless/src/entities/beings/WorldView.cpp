/**
* @file    WorldView.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the WorldView class.
*/

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
		: _region{being.region()}
		, _bounds{boost::none}
	{
		Region const& region = _region;
		RegionTileCoords coords = being.coords();
		Vision vision = being.stats.vision;
		double acuity = vision.acuity();
		double ideal_light = vision.ideal_light();
		double light_tolerance = vision.light_tolerance();

		// Calculate the maximum distance the being can see.
		int visual_range = static_cast<int>(sqrt(acuity / Vision::distance_factor));

		// Find the set of coordinates of sections plausibly visible to the being.
		set<RegionSectionCoords> section_coords_set;
		for (int r = -visual_range; r <= visual_range; ++r) {
			for (int q = -visual_range; q <= visual_range; ++q) {
				RegionTileCoords offset{q, r};
				if (offset.length() > visual_range) continue;
				RegionSectionCoords section_coords = region.containing_section_coords(coords + offset);
				if (region.section_exists(section_coords)) {
					section_coords_set.insert(section_coords);
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (RegionSectionCoords section_coords : section_coords_set) {
			SectionView section_view;
			section_view.coords = section_coords;

			for (int r = -Section::radius; r <= Section::radius; ++r) {
				for (int q = -Section::radius; q <= Section::radius; ++q) {
					auto region_tile_coords = Section::region_tile_coords(section_coords, SectionTileCoords{q, r});

					bool in_front;
					auto offset = region_tile_coords - being.coords();
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
						double light_level = region.light_level(region_tile_coords);
						double vision_divisor = 1.0 + (light_level - ideal_light) * (light_level - ideal_light) / light_tolerance * Vision::light_factor;
						int distance = coords.distance_to(region_tile_coords);

						double tile_visibility = in_front ? (acuity - distance * distance * Vision::distance_factor) / vision_divisor : 0.0;

						// Update bounding rectangle.
						if (find_bounds && tile_visibility > 0.0) {
							GamePoint tile_game_point = Layout::dflt().to_world(region_tile_coords);
							if (!_bounds) {
								_bounds = GameRect{tile_game_point.x, tile_game_point.y, 0.0, 0.0};
							} else {
								_bounds->extend(tile_game_point);
							}
						}
						section_view.tile_visibilities[r + Section::radius][q + Section::radius] = tile_visibility;
					} else {
						section_view.tile_visibilities[r + Section::radius][q + Section::radius] = 0.0;
					}
				}
			}

			_section_views.push_back(section_view);

			// Calculate being visibilities.
			for (Being const& other_being : region.beings(section_coords)) {
				RegionTileCoords other_coords = other_being.coords();
				if (other_coords.distance_to(coords) < visual_range) {
					SectionTileIndex other_tile_index = Section::tile_index(other_coords);
					double tile_visibility = section_view.tile_visibilities[other_tile_index.i][other_tile_index.j];
					
					if (tile_visibility >= _low_perception_threshold) {
						BeingView being_view;
						if (tile_visibility < _medium_perception_threshold) {
							being_view.perception = BeingView::Perception::low;
						} else if (tile_visibility < _high_perception_threshold) {
							being_view.perception = BeingView::Perception::medium;
						} else if (tile_visibility < _full_perception_threshold) {
							being_view.perception = BeingView::Perception::high;
						} else {
							being_view.perception = BeingView::Perception::full;
						}
						being_view.id = other_being.id();
						_being_views.push_back(being_view);
					}
				}
			}

			// Calculate object visibilities.
			for (Object const& object : region.objects(section_coords)) {
				RegionTileCoords other_coords = object.coords();
				if (other_coords.distance_to(coords) < visual_range) {
					SectionTileIndex other_tile_index = Section::tile_index(other_coords);
					double tile_visibility = section_view.tile_visibilities[other_tile_index.i][other_tile_index.j];

					if (tile_visibility >= _low_perception_threshold) {
						ObjectView object_view;
						if (tile_visibility < _medium_perception_threshold) {
							object_view.perception = ObjectView::Perception::low;
						} else if (tile_visibility < _high_perception_threshold) {
							object_view.perception = ObjectView::Perception::medium;
						} else if (tile_visibility < _full_perception_threshold) {
							object_view.perception = ObjectView::Perception::high;
						} else {
							object_view.perception = ObjectView::Perception::full;
						}
						object_view.id = object.id();
						_object_views.push_back(object_view);
					}
				}
			}
		}
		if (_bounds) {
			/// @todo Figure out the correct way to compute these adjustments.

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
