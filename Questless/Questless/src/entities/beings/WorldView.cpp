/**
* @file    WorldView.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the WorldView class.
*/

#include <set>
using std::set;

#include "entities/beings/WorldView.h"
#include "Game.h"
#include "entities/beings/Being.h"

namespace questless
{
	WorldView::WorldView(const Game& game, const Being& being) : _region{game.region()}
	{
		HexCoords coords = being.coords();
		double vision = being.vision();
		double light_affinity = being.light_affinity();

		// Calculate the maximum distance the being can see.
		int visual_range = static_cast<int>(sqrt(vision / BeingK::vision_distance_factor));

		// Find the set of coordinates of sections plausibly visible to the being.
		set<HexCoords> section_coords_set;
		for (int r = -visual_range; r <= visual_range; ++r) {
			for (int q = -visual_range; q <= visual_range; ++q) {
				HexCoords offset{q, r};
				if (offset.length() > visual_range) continue;
				HexCoords section_coords = _region.containing_section_coords(coords + offset);
				if (_region.section_exists(section_coords)) {
					section_coords_set.insert(section_coords);
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (HexCoords section_coords : section_coords_set) {
			SectionView section_view;
			section_view.coords = section_coords;

			for (int r = -section_radius; r <= section_radius; ++r) {
				for (int q = -section_radius; q <= section_radius; ++q) {
					HexCoords tile_coords = HexCoords{q, r} +section_coords * section_diameter;

					double light_level = _region.light_level(tile_coords);
					double vision_divisor = 1 + abs(light_level - light_affinity) * BeingK::vision_affinity_factor;
					int distance = coords.distance_to(tile_coords);
					double tile_visibility = (vision - distance * distance * BeingK::vision_distance_factor) / vision_divisor;

					section_view.tile_visibilities[r + section_radius][q + section_radius] = tile_visibility;
				}
			}

			_section_views.push_back(section_view);

			for (const Being::ptr& other_being : _region.beings(section_coords)) {
				if (other_being->coords().distance_to(coords) <= visual_range) {
					HexCoords other_coords = other_being->coords();
					BeingView being_view;
					being_view.being = other_being.get();

					double tile_visibility = section_view.tile_visibilities[other_coords.r + section_radius][other_coords.q + section_radius];
					if (tile_visibility < WorldViewK::low_perception_threshold) {
						being_view.perception = BeingView::Perception::none;
					} else if (tile_visibility < WorldViewK::medium_perception_threshold) {
						being_view.perception = BeingView::Perception::low;
					} else if (tile_visibility < WorldViewK::high_perception_threshold) {
						being_view.perception = BeingView::Perception::medium;
					} else if (tile_visibility < WorldViewK::full_perception_threshold) {
						being_view.perception = BeingView::Perception::high;
					} else {
						being_view.perception = BeingView::Perception::full;
					}

					_being_views.push_back(being_view);
				}
			}
		}
	}
}