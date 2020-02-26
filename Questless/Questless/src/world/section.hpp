//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "coordinates.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <array>
#include <optional>
#include <unordered_map>

namespace ql {
	struct light_source;

	//! An rhomboid section of hexes in a region.
	struct section {
		//! Generates a new section.
		//! @param region_id The ID of the region containing this section.
		//! @param coords The coordinates of the section within its region.
		section(id region_id, section_hex::point coords);

		//! The hex coordinates of this section within its region's sections.
		auto section_coords() const;

		//! The coordinates of this section's center tile.
		auto center_coords() const -> tile_hex::point;

		//! A map in this section of tile coordinates to occupying entities.
		auto entity_id_map() const -> std::unordered_map<tile_hex::point, id> const&;

		//! The ID of the entity at @p tile_coords or nullopt if there is none.
		auto entity_id_at(tile_hex::point tile_coords) const -> std::optional<id>;

		//! Tries to add the given entity to the section. Returns true on success or false if there is already an entity
		//! at the entity's coordinates.
		[[nodiscard]] auto try_add(id being_id) -> bool;

		//! Removes the being at the given region tile coordinates, if present.
		auto remove_at(tile_hex::point coords) -> void;

		//! Removes the entity with ID @p entity_id from this section, if present.
		auto remove(id entity_id) -> void;

		//! The ID of the tile at @p coords in this section.
		//! @note Behavior is undefined if @p coords is not within this section.
		auto tile_id_at(tile_hex::point coords) const -> id;

	private:
		//! A q-major array of tiles, representing a rhomboid section of tiles centered on this section's hex coordinates.
		std::array<std::array<id, section_diameter.value>, section_diameter.value> _tile_ids;

		std::unordered_map<tile_hex::point, id> _entity_id_map;

		//! The hex coordinates of this section within its region.
		section_hex::point _coords;

		//! The array indices of the tile at @p coords.
		//! @note Behavior is undefined if @p coords is not within this section.
		auto indices(tile_hex::point coords) const -> std::tuple<size_t, size_t>;
	};
}
