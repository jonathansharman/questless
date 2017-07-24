//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/WorldView.h"

#include <set>
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
		std::set<RegionSection::Point> section_coords_set;
		for (int q = -visual_range; q <= visual_range; ++q) {
			for (int r = -visual_range; r <= visual_range; ++r) {
				RegionTile::Vector offset{q, r};
				if (offset.length() <= visual_range) {
					if (Section const* section = region.containing_section(being.coords + offset)) {
						section_coords_set.insert(section->coords());
					}
				}
			}
		}

		// Calculate tile visibilities for each section.
		for (RegionSection::Point section_coords : section_coords_set) {
			SectionView section_view;
			section_view.coords = section_coords;

			for (int q = 0; q < Section::diameter; ++q) {
				for (int r = 0; r < Section::diameter; ++r) {
					auto region_tile_coords = Section::region_tile_coords(section_coords, SectionTile::Point{q, r});

					Perception tile_perception = being.perception_of(region_tile_coords);

					if (find_bounds && tile_perception.level > 0.0) {
						// Update bounding rectangle.
						GameSpace::Point tile_game_point = Layout::dflt().to_world(region_tile_coords);
						if (!_bounds) {
							_bounds = GameSpace::Box{GameSpace::Point{tile_game_point.x(), tile_game_point.y()}, GameSpace::Vector::zero()};
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
					_entity_views.emplace_back(other_being.id, Perception::maximum());
				} else {
					RegionTile::Point other_coords = other_being.coords;
					if ((being.coords - other_coords).length() <= visual_range) {
						SectionTile::Point other_section_coords = Section::section_tile_coords(other_coords);
						Perception tile_perception = section_view.tile_perceptions[other_section_coords.q][other_section_coords.r];

						_entity_views.emplace_back(other_being.id, tile_perception);
					}
				}
			}

			// Calculate object visibilities.
			for (Object const& object : region.section(section_coords)->objects) {
				RegionTile::Point other_coords = object.coords;
				if ((being.coords - other_coords).length() <= visual_range) {
					SectionTile::Point other_section_coords = Section::section_tile_coords(other_coords);
					Perception tile_perception = section_view.tile_perceptions[other_section_coords.q][other_section_coords.r];
					_entity_views.emplace_back(object.id, tile_perception);
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

			GameSpace::Vector size = Layout::dflt().size;
			GameSpace::Vector double_size = 2 * Layout::dflt().size;
			left(*_bounds) -= 31.0;
			top(*_bounds) -= 19.0;
			width(*_bounds) += 62.0;
			height(*_bounds) += 39.0;
		}
	}
}
