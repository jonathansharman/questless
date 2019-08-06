//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "coordinates.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace ql {
	struct light_source;

	//! An rhomboid section of hexes in a region.
	struct section {
		std::unordered_map<region_tile::point, id> entity_id_map;

		//! The coordinates of @p region_tile_coords relative to the section that contains them.
		static auto section_tile_coords(region_tile::point region_tile_coords) {
			return section_tile::point{
				((region_tile_coords.q + section_radius) % section_diameter.value + section_diameter) % section_diameter.value,
				((region_tile_coords.r + section_radius) % section_diameter.value + section_diameter) % section_diameter.value};
		}

		//! Generates a new section.
		//! @param region_tile The ID of the region containing this section.
		//! @param coords The coordinates of the section within its region.
		section(id region_tile, region_section::point coords);

		//! The hex coordinates of this section within the region's sections.
		region_section::point coords() const {
			return _coords;
		}

		//! The ID of the entity at @p tile_coords or nullopt if there is none.
		std::optional<id> entity_id_at(region_tile::point tile_coords) const;

		//! Tries to add the given entity to the section. Returns true on success or false if there is already an entity
		//! at the entity's coordinates.
		[[nodiscard]] bool try_add(id being_id);

		//! Removes the being at the given region tile coordinates, if present.
		void remove_at(region_tile::point coords);

		//! Removes the entity with ID @p entity_id from the section, if present.
		void remove(id entity_id);

		//! Region tile coordinates from @p region_section_coords and @p section_tile_coords.
		static region_tile::point region_tile_coords(region_section::point region_section_coords,
			section_tile::point section_tile_coords) {
			span q = region_section_coords.q.value * section_diameter + section_tile_coords.q - section_radius;
			span r = region_section_coords.r.value * section_diameter + section_tile_coords.r - section_radius;
			return region_tile::point{q, r};
		}

		//! Region tile coordinates from @p section_tile_coords in this section.
		region_tile::point region_tile_coords(section_tile::point section_tile_coords) const {
			return region_tile_coords(_coords, section_tile_coords);
		}

		//! The ID of the tile at @p section_tile_coords in this section.
		id tile_id_at(section_tile::point section_tile_coords) const {
			return _tile_ids[section_tile_coords.q.value][section_tile_coords.r.value];
		}

	private:
		//! A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex
		//! coordinates.
		std::array<std::array<id, section_diameter.value>, section_diameter.value> _tile_ids;
		//! The hex coordinates of the section within the region. The section's center's region tile coordinates are
		//! _coords * section_diameter.
		region_section::point _coords;
	};
}
