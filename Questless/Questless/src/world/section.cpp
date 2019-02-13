//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "section.hpp"

#include "tile.hpp"

#include "entities/beings/being.hpp"
#include "entities/objects/object.hpp"
#include "game.hpp"
#include "magic/spell.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

#include <fstream>

namespace ql {
	ref<being> section::being_entry_to_ref(std::pair<region_tile::point const, id<being>> being_entry) {
		return the_game().beings.ref(being_entry.second);
	}
	cref<being> section::being_entry_to_cref(std::pair<region_tile::point const, id<being>> being_entry) {
		return the_game().beings.ref(being_entry.second);
	}

	ref<object> section::object_entry_to_ref(std::pair<region_tile::point const, id<object>> object_entry) {
		return the_game().objects.ref(object_entry.second);
	}
	cref<object> section::object_entry_to_cref(std::pair<region_tile::point const, id<object>> object_entry) {
		return the_game().objects.ref(object_entry.second);
	}

	ref<light_source> section::light_source_id_to_ref(id<light_source> light_source_id) {
		return the_game().light_sources.ref(light_source_id);
	}
	cref<light_source> section::light_source_id_to_cref(id<light_source> light_source_id) {
		return the_game().light_sources.ref(light_source_id);
	}

	region_section::point section::region_section_coords(region_tile::point region_tile_coords) {
		auto q = region_tile_coords.q >= 0_span
					 ? 1_section_span * (region_tile_coords.q + section::radius) / section::diameter
					 : 1_section_span * (region_tile_coords.q - section::radius) / section::diameter;
		auto r = region_tile_coords.r >= 0_span
					 ? 1_section_span * (region_tile_coords.r + section::radius) / section::diameter
					 : 1_section_span * (region_tile_coords.r - section::radius) / section::diameter;
		return region_section::point{q, r};
	}

	section::section(region_section::point coords, std::istream& data_stream)
		: beings{_being_map}, objects{_object_map}, light_sources{_light_source_ids}, _coords{coords} {
		for (auto& slice : _tiles) {
			for (auto& tile : slice) {
				int c;
				temperature temperature_offset;

				data_stream >> c >> temperature_offset.value;

				switch (static_cast<tile_subtype>(c)) {
					case tile_subtype::dirt:
						tile = umake<dirt_tile>(temperature_offset);
						break;
					case tile_subtype::edge:
						tile = umake<edge_tile>(temperature_offset);
						break;
					case tile_subtype::grass:
						tile = umake<grass_tile>(temperature_offset);
						break;
					case tile_subtype::sand:
						tile = umake<sand_tile>(temperature_offset);
						break;
					case tile_subtype::snow:
						tile = umake<snow_tile>(temperature_offset);
						break;
					case tile_subtype::stone:
						tile = umake<stone_tile>(temperature_offset);
						break;
					case tile_subtype::water:
						tile = umake<water_tile>(temperature_offset);
						break;
					default:
						assert(false && "Unrecognized tile type.");
				}
			}
		}
	}

	section::section(section&& that)
		: beings{_being_map}
		, objects{_object_map}
		, light_sources{_light_source_ids}
		, _tiles{std::move(that._tiles)}
		, _coords{that._coords}
		, _being_map{std::move(that._being_map)}
		, _object_map{std::move(that._object_map)} {}

	section& section::operator=(section&& that) {
		beings.container = _being_map;
		objects.container = _object_map;
		light_sources.container = _light_source_ids;
		_tiles = std::move(that._tiles);
		_coords = that._coords;
		_being_map = std::move(that._being_map);
		_object_map = std::move(that._object_map);
		return *this;
	}

	section::~section() {}

	void section::save(char const* filename) {
		std::ofstream fout{filename};
		for (auto& r_row : _tiles) {
			for (auto& tile : r_row) {
				fout << static_cast<char>(tile->subtype()) << ' ' << tile->temperature_offset.value << ' ';
			}
		}
	}

	std::optional<id<being>> section::being_id(region_tile::point tile_coords) const {
		auto it = _being_map.find(tile_coords);
		return it != _being_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	std::optional<id<object>> section::object_id(region_tile::point tile_coords) const {
		auto it = _object_map.find(tile_coords);
		return it != _object_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	bool section::try_add(being& being) {
		being.section = this;
		auto result = _being_map.insert({being.coords, being.id});
		return result.second;
	}

	bool section::try_add(object& object) {
		object.section = this;
		auto result = _object_map.insert({object.coords, object.id});
		return result.second;
	}

	typename void section::remove_being(region_tile::point coords) {
		auto it = _being_map.find(coords);
		if (it != _being_map.end()) {
			if (being* removed_being = the_game().beings.ptr(it->second)) { removed_being->section = nullptr; }
			_being_map.erase(it);
		}
	}

	typename void section::remove_object(region_tile::point coords) {
		auto it = _object_map.find(coords);
		if (it != _object_map.end()) {
			if (object* removed_object = the_game().objects.ptr(it->second)) { removed_object->section = nullptr; }
			_object_map.erase(it);
		}
	}

	void section::remove(being& being) {
		remove_being(being.coords);
	}

	void section::remove(object& object) {
		remove_object(object.coords);
	}

	void section::add(light_source const& light_source) {
		_light_source_ids.insert(light_source.id);
	}

	void section::remove(light_source const& light_source) {
		_light_source_ids.erase(light_source.id);
	}
}
