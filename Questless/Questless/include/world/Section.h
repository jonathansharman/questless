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
		void save(std::string const& filename);

		/// @return The hex coordinates of the section within the region's sections.
		RegionSectionCoords coords() const { return _coords; }

		/// @return A list of beings in this section.
		std::vector<Being::ref> beings() const;

		/// @return A list objects in this section.
		std::vector<Object::ref> objects() const;

		/// @param tile_coords Tile coordinates within the region.
		/// @return The ID of the being at the given tile coordinates or nullopt if there is none.
		std::optional<Id<Being>> being_id(RegionTileCoords tile_coords) const;

		/// @param tile_coords Tile coordinates within the region.
		/// @return The ID of the object at the given tile coordinates or nullopt if there is none.
		std::optional<Id<Object>> object_id(RegionTileCoords tile_coords) const;

		/// Adds the given being to the section. Throws a logic_error if there is already a being at its coordinates.
		////
		void add(Being& being)
		{
			being.section = this;
			auto result = _being_ids.insert({being.coords, being.id});
			if (!result.second) {
				throw std::logic_error{"Attempted to place a being on top of another in a section."};
			}
		}

		/// Adds the given object to the section. Throws a logic_error if there is already an object at its coordinates.
		////
		void add(Object& object)
		{
			object.section = this;
			auto result = _object_ids.insert({object.coords, object.id});
			if (!result.second) {
				throw std::logic_error{"Attempted to place an object on top of another in a section."};
			}
		}

		/// Removes the being at the given region tile coordinates, if present.
		////
		void remove_being(RegionTileCoords coords);

		/// Removes the object at the given region tile coordinates, if present.
		void remove_object(RegionTileCoords coords);

		/// Removes the given being from the section.
		typename void remove(Being& being)
		{
			remove_being(being.coords);
		}

		/// Removes the given object from the section.
		typename void remove(Object& object)
		{
			remove_object(object.coords);
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
		Tile const& tile(SectionTileCoords tile_coords) const
		{
			SectionTileIndex index = tile_index(tile_coords);
			return *_tiles[index.i][index.j];
		}
		/// @param region_tile_coords Hex coordinates of a tile relative to the region.
		/// @return The tile at the given region tile coordinates.
		Tile const& tile(RegionTileCoords region_tile_coords) const
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

		std::unordered_map<RegionTileCoords, Id<Being>> _being_ids;
		std::unordered_map<RegionTileCoords, Id<Object>> _object_ids;

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Being>::value>>
		std::unordered_map<RegionTileCoords, Being::ptr>& entities() { return _beings; }

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Object>::value>>
		std::unordered_map<RegionTileCoords, Id<Object>>& entities() { return _objects; }

		/// Reads section data from a stream.
		/// @param in The stream from which to read the data.
		void read(std::istream& in);
	};
}
