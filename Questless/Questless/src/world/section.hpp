//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include "vecx/hex_space.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace ql {
	struct being;
	struct light_source;
	struct object;
	struct tile;

	//! An rhomboid section of hexes in a region.
	struct section {
		static constexpr span radius = 10_span;
		static constexpr span diameter = 2 * radius + 1_span;

		//! The coordinates of the section that contains the tile at @p region_tile_coords.
		static region_section::point region_section_coords(region_tile::point region_tile_coords);

		//! The coordinates of @p region_tile_coords relative to the section that contains them.
		static section_tile::point section_tile_coords(region_tile::point region_tile_coords) {
			return section_tile::point{((region_tile_coords.q + section::radius) % section::diameter.value + section::diameter) %
										   section::diameter.value,
				((region_tile_coords.r + section::radius) % section::diameter.value + section::diameter) %
					section::diameter.value};
		}

		std::unordered_map<region_tile::point, ent> being_id_map;
		std::unordered_map<region_tile::point, ent> object_id_map;
		std::unordered_set<ent> light_source_ids;

		//! @param coords The coordinates of the section within its region.
		//! @param data_stream A stream of section data.
		section(region_section::point coords, std::istream& data_stream);

		section(section const& that) = delete;
		section(section&& that);

		section& operator=(section&) = delete;
		section& operator=(section&& that);

		~section();

		//! Saves the section to the specified file.
		//! @param filename The section's filename.
		void save(char const* filename);

		//! The hex coordinates of this section within the region's sections.
		region_section::point coords() const {
			return _coords;
		}

		//! The ID of the entity at @p tile_coords or nullopt if there is none.
		std::optional<ent> entity_id_at(region_tile::point tile_coords) const;

		//! Tries to add the given being to the section. Returns true on success or false
		//! if there is already a being at the being's coordinates.
		[[nodiscard]] bool try_add(being& being);

		//! Tries to add the given object to the section. Returns true on success or false
		//! if there is already an object at the object's coordinates.
		[[nodiscard]] bool try_add(object& object);

		//! Removes the being at the given region tile coordinates, if present.
		void remove_being(region_tile::point coords);

		//! Removes the object at the given region tile coordinates, if present.
		void remove_object(region_tile::point coords);

		//! Removes @p being from the section, if present.
		void remove(being& being);

		//! Removes @p object from the section, if present.
		void remove(object& object);

		//! Region tile coordinates from @p region_section_coords and @p section_tile_coords.
		static region_tile::point region_tile_coords(region_section::point region_section_coords,
			section_tile::point section_tile_coords) {
			span q = region_section_coords.q.value * diameter + section_tile_coords.q - radius;
			span r = region_section_coords.r.value * diameter + section_tile_coords.r - radius;
			return region_tile::point{q, r};
		}

		//! Region tile coordinates from @p section_tile_coords in this section.
		region_tile::point region_tile_coords(section_tile::point section_tile_coords) const {
			return region_tile_coords(_coords, section_tile_coords);
		}

		//! The tile at @p section_tile_coords in this section.
		ql::tile const& tile_at(section_tile::point section_tile_coords) const {
			return *_tiles[section_tile_coords.q.value][section_tile_coords.r.value];
		}
		//! The tile at @p section_tile_coords in this section.
		ql::tile& tile_at(section_tile::point section_tile_coords) {
			return *_tiles[section_tile_coords.q.value][section_tile_coords.r.value];
		}

	private:
		//! A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex
		//! coordinates.
		std::array<std::array<uptr<ql::tile>, diameter.value>, diameter.value> _tiles;
		//! The hex coordinates of the section within the region. The section's center's region tile coordinates are
		//! _coords * section_diameter.
		region_section::point _coords;
	};
}
