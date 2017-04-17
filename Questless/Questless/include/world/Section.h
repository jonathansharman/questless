//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "coordinates.h"
#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "units/HexCoords.h"
#include "world/Tile.h"
#include "animation/Camera.h"

namespace questless
{
	//! An rhomboid section of hexes in a region.
	class Section
	{
	public:
		using ref = std::reference_wrapper<Section>;
		using cref = std::reference_wrapper<const Section>;

		static constexpr int radius = 10;
		static constexpr int diameter = 2 * radius + 1;

		//! The coordinates of the section that contains the tile at @p region_tile_coords.
		static RegionSectionCoords region_section_coords(RegionTileCoords region_tile_coords);

		//! The coordinates of @p region_tile_coords relative to the section that contains them.
		static SectionTileCoords section_tile_coords(RegionTileCoords region_tile_coords)
		{
			return SectionTileCoords
				{ ((region_tile_coords.q + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
				, ((region_tile_coords.r + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
				};
		}

		//! @param coords The coordinates of the section within its region.
		//! @param data_stream A stream of section data.
		Section(RegionSectionCoords coords, std::istream& data_stream);

		//! Saves the section to the specified file.
		//! @param filename The section's filename.
		void save(char const* filename);

		//! The hex coordinates of this section within the region's sections.
		RegionSectionCoords coords() const { return _coords; }

		//! A list of beings in this section.
		std::vector<Being::cref> beings() const;
		//! A list of beings in this section.
		std::vector<Being::ref> beings();

		//! A list objects in this section.
		std::vector<Object::cref> objects() const;
		//! A list objects in this section.
		std::vector<Object::ref> objects();

		//! The ID of the being at @p tile_coords or nullopt if there is none.
		std::optional<Id<Being>> being_id(RegionTileCoords tile_coords) const;

		//! The ID of the object at @p tile_coords or nullopt if there is none.
		std::optional<Id<Object>> object_id(RegionTileCoords tile_coords) const;

		//! Adds the given being to the section. Throws a logic_error if there is already a being at its coordinates.
		void add(Being& being)
		{
			being.section = this;
			auto result = _being_ids.insert({being.coords, being.id});
			if (!result.second) {
				throw std::logic_error{"Attempted to place a being on top of another in a section."};
			}
		}

		//! Adds the given object to the section. Throws a logic_error if there is already an object at its coordinates.
		void add(Object& object)
		{
			object.section = this;
			auto result = _object_ids.insert({object.coords, object.id});
			if (!result.second) {
				throw std::logic_error{"Attempted to place an object on top of another in a section."};
			}
		}

		//! Removes the being at the given region tile coordinates, if present.
		void remove_being(RegionTileCoords coords);

		//! Removes the object at the given region tile coordinates, if present.
		void remove_object(RegionTileCoords coords);

		//! Removes the given being from the section.
		typename void remove(Being& being)
		{
			remove_being(being.coords);
		}

		//! Removes the given object from the section.
		typename void remove(Object& object)
		{
			remove_object(object.coords);
		}

		//! Region tile coordinates from @p region_section_coords and @p section_tile_coords.
		static RegionTileCoords region_tile_coords(RegionSectionCoords region_section_coords, SectionTileCoords section_tile_coords)
		{
			int q = region_section_coords.q * diameter + section_tile_coords.q - radius;
			int r = region_section_coords.r * diameter + section_tile_coords.r - radius;
			return RegionTileCoords{q, r};
		}

		//! Region tile coordinates from @p section_tile_coords in this section.
		RegionTileCoords region_tile_coords(SectionTileCoords section_tile_coords) const
		{
			return region_tile_coords(_coords, section_tile_coords);
		}

		//! The tile at @p section_tile_coords in this section.
		Tile const& tile(SectionTileCoords section_tile_coords) const
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
		//! The tile at @p section_tile_coords in this section.
		Tile& tile(SectionTileCoords section_tile_coords)
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
	private:
		std::array<std::array<std::unique_ptr<Tile>, diameter>, diameter> _tiles; //!< A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		RegionSectionCoords _coords; //!< The hex coordinates of the section within the region. The section's center's region tile coordinates are _coords * section_diameter.

		std::unordered_map<RegionTileCoords, Id<Being>> _being_ids;
		std::unordered_map<RegionTileCoords, Id<Object>> _object_ids;

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Being>::value>>
		std::unordered_map<RegionTileCoords, Being::uptr>& entities() { return _beings; }

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Object>::value>>
		std::unordered_map<RegionTileCoords, Id<Object>>& entities() { return _objects; }

		//! Reads section data from a stream.
		//! @param in The stream from which to read the data.
		void read(std::istream& in);
	};
}
