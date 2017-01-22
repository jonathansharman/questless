/**
* @file    Section.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Section class, which represents a hexagonal collection of hexes in the world.
*/

#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "coordinates.h"
#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "units/HexCoords.h"
#include "world/Tile.h"
#include "animation/Camera.h"

namespace questless
{
	class Section
	{
	public:
		using ref = std::reference_wrapper<Section>;
		using cref = std::reference_wrapper<const Section>;

		static constexpr int radius = 10;
		static constexpr int diameter = 2 * radius + 1;

		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return The tile index in the section of the given section coordinates.
		static SectionTileIndex tile_index(SectionTileCoords tile_coords);

		/// @param region_tile_coords Hex coordinates of a tile relative to the region.
		/// @return Positive hex coordinates relative to the section containing the given region coordinates.
		static SectionTileIndex tile_index(RegionTileCoords region_tile_coords);

		/// @param coords The positive coordinates of the section within its region.
		/// @param data_stream A stream of section data.
		Section(RegionSectionCoords coords, std::istream& data_stream);

		/// Saves the section to the specified file.
		/// @param filename The section's filename.
		void save(const std::string& filename);

		/// @return The hex coordinates of the section within the region's sections.
		RegionSectionCoords coords() const { return _coords; }

		/// @return The list of beings in this section.
		std::vector<Being::ref> beings() const;

		/// @return The list of objects in this section.
		std::vector<Object::ref> objects() const;

		/// @param tile_coords Tile coordinates within the region.
		/// @return The being at the given tile coordinates or nullptr if none.
		Being* being(RegionTileCoords tile_coords) const;

		/// @param tile_coords Tile coordinates within the region.
		/// @return The object at the given tile coordinates or nullptr if none.
		Object* object(RegionTileCoords tile_coords) const;

		/// Adds the given entity to the section.
		/// @param being An entity to add.
		/// @tparam EntityType The type of entity to add. Possible values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity)
		{
			entity->section(this);
			RegionTileCoords coords = entity->coords();
			entities<EntityType>()[coords] = std::move(entity);
		}

		/// Removes the entity of the given type at the given region tile coordinates.
		/// @param coords Coordinates in the region of the entity to be removed.
		/// @return The unique pointer to the removed entity or nullptr if none found.
		/// @tparam EntityType The type of entity to remove. Possible values are Being and Object.
		template <typename EntityType>
		typename EntityType::ptr remove(RegionTileCoords coords)
		{
			auto it = entities<EntityType>().find(coords);
			if (it != entities<EntityType>().end()) {
				EntityType::ptr removed_entity = std::move(it->second);
				entities<EntityType>().erase(it);
				removed_entity->section(nullptr);
				return removed_entity;
			} else {
				return nullptr;
			}
		}

		/// Removes the given entity from the section.
		/// @param entity A reference to the entity to remove.
		/// @return The unique pointer to the removed entity or nullptr if not present.
		/// @tparam EntityType The type of entity to remove. Possible values are Being and Object.
		template <typename EntityType>
		typename EntityType::ptr remove(typename const EntityType& entity)
		{
			return remove<EntityType>(entity.coords());
		}

		/// @param section_coords Hex coordinates of a section relative to the region.
		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return Hex coordinates relative to the region.
		static RegionTileCoords region_tile_coords(RegionSectionCoords section_coords, SectionTileCoords tile_coords)
		{
			int q = section_coords.q * diameter + tile_coords.q;
			int r = section_coords.r * diameter + tile_coords.r;
			return RegionTileCoords{q, r};
		}
		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return Hex coordinates relative to the region.
		RegionTileCoords region_tile_coords(SectionTileCoords tile_coords) const
		{
			return region_tile_coords(_coords, tile_coords);
		}

		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return The tile at the given section tile coordinates.
		const Tile& tile(SectionTileCoords tile_coords) const
		{
			SectionTileIndex index = tile_index(tile_coords);
			return *_tiles[index.i][index.j];
		}
		/// @param region_tile_coords Hex coordinates of a tile relative to the region.
		/// @return The tile at the given region tile coordinates.
		const Tile& tile(RegionTileCoords region_tile_coords) const
		{
			SectionTileIndex index = tile_index(region_tile_coords);
			return *_tiles[index.i][index.j];
		}

		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return The light level at the given section coordinates.
		double light_level(SectionTileCoords tile_coords) const { return tile(tile_coords).light_level(); }
		/// @param tile_coords Hex coordinates of a tile relative to the region.
		/// @return The light level at the given region coordinates.
		double light_level(RegionTileCoords region_tile_coords) const { return tile(region_tile_coords).light_level(); }

		/// @param tile_coords Hex coordinates of a tile relative to the section.
		/// @return The temperature at the given section coordinates.
		double temperature(SectionTileCoords tile_coords) const { return tile(tile_coords).temperature(); }
		/// @param tile_coords Hex coordinates of a tile relative to the region.
		/// @return The temperature at the given region coordinates.
		double temperature(RegionTileCoords region_tile_coords) const { return tile(region_tile_coords).temperature(); }
	private:
		std::array<std::array<std::unique_ptr<Tile>, diameter>, diameter> _tiles; ///< An r-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		RegionSectionCoords _coords; ///< The hex coordinates of the section within the region. The section's center's region tile coordinates are _coords * section_diameter.

		std::unordered_map<RegionTileCoords, Being::ptr> _beings;
		std::unordered_map<RegionTileCoords, Object::ptr> _objects;

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Being>::value>>
		std::unordered_map<RegionTileCoords, Being::ptr>& entities() { return _beings; }

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Object>::value>>
		std::unordered_map<RegionTileCoords, Object::ptr>& entities() { return _objects; }

		/// Reads section data from a stream.
		/// @param in The stream from which to read the data.
		void read(std::istream& in);
	};
}
