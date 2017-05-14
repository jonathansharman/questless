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
#include "utility/ContainerView.h"
#include "utility/Id.h"
#include "utility/reference.h"

namespace questless
{
	class Being;

	class Object;

	class LightSource;

	//! An rhomboid section of hexes in a region.
	class Section
	{
	public:
		static ref<Being> being_entry_to_ref(std::pair<RegionTile::Point const, Id<Being>> being_entry);
		static cref<Being> being_entry_to_cref(std::pair<RegionTile::Point const, Id<Being>> being_entry);
		using BeingsView = ContainerView
			< std::unordered_map<RegionTile::Point, Id<Being>>
			, ref<Being>
			, cref<Being>
			, being_entry_to_ref
			, being_entry_to_cref
			>;

		static ref<Object> object_entry_to_ref(std::pair<RegionTile::Point const, Id<Object>> object_entry);
		static cref<Object> object_entry_to_cref(std::pair<RegionTile::Point const, Id<Object>> object_entry);
		using ObjectsView = ContainerView
			< std::unordered_map<RegionTile::Point, Id<Object>>
			, ref<Object>
			, cref<Object>
			, object_entry_to_ref
			, object_entry_to_cref
			>;

		static ref<LightSource> light_source_id_to_ref(Id<LightSource> light_source_id);
		static cref<LightSource> light_source_id_to_cref(Id<LightSource> light_source_id);
		using LightSourcesView = ContainerView
			< std::unordered_set<Id<LightSource>>
			, ref<LightSource>
			, cref<LightSource>
			, light_source_id_to_ref
			, light_source_id_to_cref
			>;

		static constexpr int radius = 10;
		static constexpr int diameter = 2 * radius + 1;

		//! The coordinates of the section that contains the tile at @p region_tile_coords.
		static RegionSection::Point region_section_coords(RegionTile::Point region_tile_coords);

		//! The coordinates of @p region_tile_coords relative to the section that contains them.
		static SectionTile::Point section_tile_coords(RegionTile::Point region_tile_coords)
		{
			return SectionTile::Point
				{ ((region_tile_coords.q + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
				, ((region_tile_coords.r + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
				};
		}

		BeingsView beings;
		ObjectsView objects;
		LightSourcesView light_sources;

		//! @param coords The coordinates of the section within its region.
		//! @param data_stream A stream of section data.
		Section(RegionSection::Point coords, std::istream& data_stream);

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
		RegionSection::Point coords() const { return _coords; }

		//! The ID of the being at @p tile_coords or nullopt if there is none.
		std::optional<Id<Being>> being_id(RegionTile::Point tile_coords) const;

		//! The ID of the object at @p tile_coords or nullopt if there is none.
		std::optional<Id<Object>> object_id(RegionTile::Point tile_coords) const;

		//! Adds the given being to the section. Throws a logic_error if there is already a being at its coordinates.
		void add(Being& being);

		//! Adds the given object to the section. Throws a logic_error if there is already an object at its coordinates.
		void add(Object& object);

		//! Removes the being at the given region tile coordinates, if present.
		void remove_being(RegionTile::Point coords);

		//! Removes the object at the given region tile coordinates, if present.
		void remove_object(RegionTile::Point coords);

		//! Removes @p being from the section.
		void remove(Being& being);

		//! Removes @p object from the section.
		void remove(Object& object);

		//! Adds @p light_source to the sections's light sources.
		void add(LightSource const& light_source);

		//! Adds @p light_source from the section's light sources, if present.
		void remove(LightSource const& light_source);

		//! Region tile coordinates from @p region_section_coords and @p section_tile_coords.
		static RegionTile::Point region_tile_coords(RegionSection::Point region_section_coords, SectionTile::Point section_tile_coords)
		{
			int q = region_section_coords.q * diameter + section_tile_coords.q - radius;
			int r = region_section_coords.r * diameter + section_tile_coords.r - radius;
			return RegionTile::Point{q, r};
		}

		//! Region tile coordinates from @p section_tile_coords in this section.
		RegionTile::Point region_tile_coords(SectionTile::Point section_tile_coords) const
		{
			return region_tile_coords(_coords, section_tile_coords);
		}

		//! The tile at @p section_tile_coords in this section.
		Tile const& tile(SectionTile::Point section_tile_coords) const
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
		//! The tile at @p section_tile_coords in this section.
		Tile& tile(SectionTile::Point section_tile_coords)
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
	private:
		//! A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		std::array<std::array<uptr<Tile>, diameter>, diameter> _tiles;
		//! The hex coordinates of the section within the region. The section's center's region tile coordinates are _coords * section_diameter.
		RegionSection::Point _coords;

		std::unordered_map<RegionTile::Point, Id<Being>> _being_map;
		std::unordered_map<RegionTile::Point, Id<Object>> _object_map;

		std::unordered_set<Id<LightSource>> _light_source_ids;
	};
}
