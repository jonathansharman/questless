//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iostream>

#include "animation/Camera.h"
#include "coordinates.h"
#include "Tile.h"
#include "units/HexCoords.h"
#include "utility/Id.h"
#include "utility/reference.h"
#include "utility/XformedContainer.h"

namespace questless
{
	class Being;

	class Object;

	class LightSource;

	//! An rhomboid section of hexes in a region.
	class Section
	{
	public:
		static ref<Being> being_entry_to_ref(std::pair<RegionTileCoords const, Id<Being>> being_entry);
		static cref<Being> being_entry_to_cref(std::pair<RegionTileCoords const, Id<Being>> being_entry);
		using BeingsView = XformedContainer
			< std::unordered_map<RegionTileCoords, Id<Being>>
			, ref<Being>
			, cref<Being>
			, being_entry_to_ref
			, being_entry_to_cref
			>;

		static ref<Object> object_entry_to_ref(std::pair<RegionTileCoords const, Id<Object>> object_entry);
		static cref<Object> object_entry_to_cref(std::pair<RegionTileCoords const, Id<Object>> object_entry);
		using ObjectsView = XformedContainer
			< std::unordered_map<RegionTileCoords, Id<Object>>
			, ref<Object>
			, cref<Object>
			, object_entry_to_ref
			, object_entry_to_cref
			>;

		static ref<LightSource> light_source_id_to_ref(Id<LightSource> light_source_id);
		static cref<LightSource> light_source_id_to_cref(Id<LightSource> light_source_id);
		using LightSourcesView = XformedContainer
			< std::unordered_set<Id<LightSource>>
			, ref<LightSource>
			, cref<LightSource>
			, light_source_id_to_ref
			, light_source_id_to_cref
			>;

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

		BeingsView beings;
		ObjectsView objects;
		LightSourcesView light_sources;

		//! @param coords The coordinates of the section within its region.
		//! @param data_stream A stream of section data.
		Section(RegionSectionCoords coords, std::istream& data_stream);

		Section(Section const& that) = delete;

		Section(Section&& that)
			: beings{_being_map}
			, objects{_object_map}
			, light_sources{_light_source_ids}
			, _tiles{std::move(that._tiles)}
			, _coords{std::move(that._coords)}
			, _being_map{std::move(that._being_map)}
			, _object_map{std::move(that._object_map)}
			, _light_source_ids{std::move(that._light_source_ids)}
		{}

		//! Saves the section to the specified file.
		//! @param filename The section's filename.
		void save(char const* filename);

		//! The hex coordinates of this section within the region's sections.
		RegionSectionCoords coords() const { return _coords; }

		//! The ID of the being at @p tile_coords or nullopt if there is none.
		std::optional<Id<Being>> being_id(RegionTileCoords tile_coords) const;

		//! The ID of the object at @p tile_coords or nullopt if there is none.
		std::optional<Id<Object>> object_id(RegionTileCoords tile_coords) const;

		//! Adds the given being to the section. Throws a logic_error if there is already a being at its coordinates.
		void add(Being& being);

		//! Adds the given object to the section. Throws a logic_error if there is already an object at its coordinates.
		void add(Object& object);

		//! Removes the being at the given region tile coordinates, if present.
		void remove_being(RegionTileCoords coords);

		//! Removes the object at the given region tile coordinates, if present.
		void remove_object(RegionTileCoords coords);

		//! Removes @p being from the section.
		void remove(Being& being);

		//! Removes @p object from the section.
		void remove(Object& object);

		//! Adds @p light_source to the sections's light sources.
		void add(LightSource const& light_source);

		//! Adds @p light_source from the section's light sources, if present.
		void remove(LightSource const& light_source);

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
		//! A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		std::array<std::array<std::unique_ptr<Tile>, diameter>, diameter> _tiles;
		//! The hex coordinates of the section within the region. The section's center's region tile coordinates are _coords * section_diameter.
		RegionSectionCoords _coords;

		std::unordered_map<RegionTileCoords, Id<Being>> _being_map;
		std::unordered_map<RegionTileCoords, Id<Object>> _object_map;

		std::unordered_set<Id<LightSource>> _light_source_ids;
	};
}
