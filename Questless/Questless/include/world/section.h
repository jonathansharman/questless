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

#include "animation/camera.h"
#include "coordinates.h"
#include "tile.h"
#include "units/hex_coords.h"
#include "utility/container_view.h"
#include "utility/id.h"
#include "utility/reference.h"

namespace ql
{
	class being;

	class object;

	class light_source;

	//! An rhomboid section of hexes in a region.
	class section
	{
	public:
		static ref<being> being_entry_to_ref(std::pair<region_tile::point const, id<being>> being_entry);
		static cref<being> being_entry_to_cref(std::pair<region_tile::point const, id<being>> being_entry);
		using beings_view = container_view
			< std::unordered_map<region_tile::point, id<being>>
			, ref<being>
			, cref<being>
			, being_entry_to_ref
			, being_entry_to_cref
			>;

		static ref<object> object_entry_to_ref(std::pair<region_tile::point const, id<object>> object_entry);
		static cref<object> object_entry_to_cref(std::pair<region_tile::point const, id<object>> object_entry);
		using objects_view = container_view
			< std::unordered_map<region_tile::point, id<object>>
			, ref<object>
			, cref<object>
			, object_entry_to_ref
			, object_entry_to_cref
			>;

		static ref<light_source> light_source_id_to_ref(id<light_source> light_source_id);
		static cref<light_source> light_source_id_to_cref(id<light_source> light_source_id);
		using light_sources_view = container_view
			< std::unordered_set<id<light_source>>
			, ref<light_source>
			, cref<light_source>
			, light_source_id_to_ref
			, light_source_id_to_cref
			>;

		static constexpr int radius = 10;
		static constexpr int diameter = 2 * radius + 1;

		//! The coordinates of the section that contains the tile at @p region_tile_coords.
		static region_section::point region_section_coords(region_tile::point region_tile_coords);

		//! The coordinates of @p region_tile_coords relative to the section that contains them.
		static section_tile::point section_tile_coords(region_tile::point region_tile_coords)
		{
			return section_tile::point
				{ ((region_tile_coords.q + section::radius) % section::diameter + section::diameter) % section::diameter
				, ((region_tile_coords.r + section::radius) % section::diameter + section::diameter) % section::diameter
				};
		}

		beings_view beings;
		objects_view objects;
		light_sources_view light_sources;

		//! @param coords The coordinates of the section within its region.
		//! @param data_stream A stream of section data.
		section(region_section::point coords, std::istream& data_stream);

		section(section const& that) = delete;

		section(section&& that)
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
		region_section::point coords() const { return _coords; }

		//! The ID of the being at @p tile_coords or nullopt if there is none.
		std::optional<id<being>> being_id(region_tile::point tile_coords) const;

		//! The ID of the object at @p tile_coords or nullopt if there is none.
		std::optional<id<object>> object_id(region_tile::point tile_coords) const;

		//! Adds the given being to the section. Throws a logic_error if there is already a being at its coordinates.
		void add(being& being);

		//! Adds the given object to the section. Throws a logic_error if there is already an object at its coordinates.
		void add(object& object);

		//! Removes the being at the given region tile coordinates, if present.
		void remove_being(region_tile::point coords);

		//! Removes the object at the given region tile coordinates, if present.
		void remove_object(region_tile::point coords);

		//! Removes @p being from the section.
		void remove(being& being);

		//! Removes @p object from the section.
		void remove(object& object);

		//! Adds @p light_source to the sections's light sources.
		void add(light_source const& light_source);

		//! Adds @p light_source from the section's light sources, if present.
		void remove(light_source const& light_source);

		//! Region tile coordinates from @p region_section_coords and @p section_tile_coords.
		static region_tile::point region_tile_coords(region_section::point region_section_coords, section_tile::point section_tile_coords)
		{
			int q = region_section_coords.q * diameter + section_tile_coords.q - radius;
			int r = region_section_coords.r * diameter + section_tile_coords.r - radius;
			return region_tile::point{q, r};
		}

		//! Region tile coordinates from @p section_tile_coords in this section.
		region_tile::point region_tile_coords(section_tile::point section_tile_coords) const
		{
			return region_tile_coords(_coords, section_tile_coords);
		}

		//! The tile at @p section_tile_coords in this section.
		ql::tile const& tile_at(section_tile::point section_tile_coords) const
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
		//! The tile at @p section_tile_coords in this section.
		ql::tile& tile_at(section_tile::point section_tile_coords)
		{
			return *_tiles[section_tile_coords.q][section_tile_coords.r];
		}
	private:
		//! A q-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		std::array<std::array<uptr<ql::tile>, diameter>, diameter> _tiles;
		//! The hex coordinates of the section within the region. The section's center's region tile coordinates are _coords * section_diameter.
		region_section::point _coords;

		std::unordered_map<region_tile::point, id<being>> _being_map;
		std::unordered_map<region_tile::point, id<object>> _object_map;

		std::unordered_set<id<light_source>> _light_source_ids;
	};
}
