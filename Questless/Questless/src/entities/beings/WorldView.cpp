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
#include "utility/hex-utilities.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"

using namespace sdl;

namespace questless
{
	WorldView::WorldView(const Being& being, bool find_bounds)
		: _region{being.region()}
		, _bounds{nullopt}
	{
		RegionTileCoords coords = being.coords();
		Vision vision = being.vision();
		double acuity = vision.acuity;
		double ideal_light = vision.ideal_light;
		double light_tolerance = vision.light_tolerance;

		// Calculate the maximum distance the being can see.
		int visual_range = static_cast<int>(sqrt(acuity / Vision::distance_factor));

		// Find the set of coordinates of sections plausibly visible to the being.
		set<RegionSectionCoords> section_coords_set;
		for (int r = -visual_range; r <= visual_range; ++r) {
			for (int q = -visual_range; q <= visual_range; ++q) {
				HexCoords offset{q, r};
				if (offset.length() > visual_range) continue;
				RegionSectionCoords section_coords = _region.containing_section_coords({coords.hex + offset});
				if (_region.section_exists(section_coords)) {
					section_coords_set.insert(section_coords);
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (RegionSectionCoords section_coords : section_coords_set) {
			SectionView section_view;
			section_view.coords = section_coords;

			for (int r = -section_radius; r <= section_radius; ++r) {
				for (int q = -section_radius; q <= section_radius; ++q) {
					RegionTileCoords tile_coords{HexCoords{q, r} + section_coords.hex * section_diameter};

					double light_level = _region.light_level(tile_coords);
					double vision_divisor = 1 + (light_level - ideal_light) * (light_level - ideal_light) / light_tolerance * Vision::light_factor;
					int distance = coords.hex.distance_to(tile_coords.hex);
					double tile_visibility = (acuity - distance * distance * Vision::distance_factor) / vision_divisor;

					// Update max and min coordinates.
					if (find_bounds && tile_visibility > 0.0) {
						Point world_coords = Layout::dflt().to_world(tile_coords.hex);
						if (!_bounds) {
							_bounds = Rect{world_coords.x, world_coords.y, 1, 1};
						} else {
							extend_bounds(*_bounds, world_coords);
						}
					}

					section_view.tile_visibilities[r + section_radius][q + section_radius] = tile_visibility;
				}
			}

			_section_views.push_back(section_view);

			// Calculate being visibilities.
			for (const Being& other_being : _region.beings(section_coords)) {
				RegionTileCoords other_coords = other_being.coords();
				if (other_coords.hex.distance_to(coords.hex) < visual_range) {
					BeingView being_view;
					being_view.id = other_being.id();

					SectionTileIndex other_tile_index = Section::tile_index(other_coords);

					double tile_visibility = section_view.tile_visibilities[other_tile_index.i][other_tile_index.j];
					if (tile_visibility < _low_perception_threshold) {
						being_view.perception = BeingView::Perception::none;
					} else if (tile_visibility < _medium_perception_threshold) {
						being_view.perception = BeingView::Perception::low;
					} else if (tile_visibility < _high_perception_threshold) {
						being_view.perception = BeingView::Perception::medium;
					} else if (tile_visibility < _full_perception_threshold) {
						being_view.perception = BeingView::Perception::high;
					} else {
						being_view.perception = BeingView::Perception::full;
					}

					_being_views.push_back(being_view);
				}
			}

			// Calculate object visibilities.
			for (const Object& object : _region.objects(section_coords)) {
				RegionTileCoords other_coords = object.coords();
				if (other_coords.hex.distance_to(coords.hex) < visual_range) {
					ObjectView object_view;
					object_view.id = object.id();

					SectionTileIndex other_tile_index = Section::tile_index(other_coords);

					double tile_visibility = section_view.tile_visibilities[other_tile_index.i][other_tile_index.j];
					if (tile_visibility < _low_perception_threshold) {
						object_view.perception = ObjectView::Perception::none;
					} else if (tile_visibility < _medium_perception_threshold) {
						object_view.perception = ObjectView::Perception::low;
					} else if (tile_visibility < _high_perception_threshold) {
						object_view.perception = ObjectView::Perception::medium;
					} else if (tile_visibility < _full_perception_threshold) {
						object_view.perception = ObjectView::Perception::high;
					} else {
						object_view.perception = ObjectView::Perception::full;
					}

					_object_views.push_back(object_view);
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

			Point size = Layout::dflt().size.to_point();
			Point double_size = (2 * Layout::dflt().size).to_point();
			_bounds->x -= 31;
			_bounds->y -= 19;
			_bounds->w += 62;
			_bounds->h += 39;
		}
	}
}
