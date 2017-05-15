//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/Section.h"

#include <fstream>

#include "Game.h"
#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "world/LightSource.h"
#include "world/Region.h"

namespace questless
{
	ref<Being> Section::being_entry_to_ref(std::pair<RegionTile::Point const, Id<Being>> being_entry)
	{
		return game().beings.ref(being_entry.second);
	}
	cref<Being> Section::being_entry_to_cref(std::pair<RegionTile::Point const, Id<Being>> being_entry)
	{
		return game().beings.ref(being_entry.second);
	}

	ref<Object> Section::object_entry_to_ref(std::pair<RegionTile::Point const, Id<Object>> object_entry)
	{
		return game().objects.ref(object_entry.second);
	}
	cref<Object> Section::object_entry_to_cref(std::pair<RegionTile::Point const, Id<Object>> object_entry)
	{
		return game().objects.ref(object_entry.second);
	}

	ref<LightSource> Section::light_source_id_to_ref(Id<LightSource> light_source_id)
	{
		return game().light_sources.ref(light_source_id);
	}
	cref<LightSource> Section::light_source_id_to_cref(Id<LightSource> light_source_id)
	{
		return game().light_sources.ref(light_source_id);
	}

	RegionSection::Point Section::region_section_coords(RegionTile::Point region_tile_coords)
	{
		int q = region_tile_coords.q >= 0
			? (region_tile_coords.q + Section::radius) / Section::diameter
			: (region_tile_coords.q - Section::radius) / Section::diameter
			;
		int r = region_tile_coords.r >= 0
			? (region_tile_coords.r + Section::radius) / Section::diameter
			: (region_tile_coords.r - Section::radius) / Section::diameter
			;
		return RegionSection::Point{q, r};
	}

	Section::Section(RegionSection::Point coords, std::istream& data_stream)
		: beings{_being_map}
		, objects{_object_map}
		, light_sources{_light_source_ids}
		, _coords{coords}
	{
		for (auto& slice : _tiles) {
			for (auto& tile : slice) {
				int c;
				double temperature_offset;

				data_stream >> c >> temperature_offset;

				switch (static_cast<Tile::TileClass>(c)) {
					case Tile::TileClass::dirt:
						tile = std::make_unique<DirtTile>(temperature_offset);
						break;
					case Tile::TileClass::edge:
						tile = std::make_unique<EdgeTile>(temperature_offset);
						break;
					case Tile::TileClass::grass:
						tile = std::make_unique<GrassTile>(temperature_offset);
						break;
					case Tile::TileClass::sand:
						tile = std::make_unique<SandTile>(temperature_offset);
						break;
					case Tile::TileClass::snow:
						tile = std::make_unique<SnowTile>(temperature_offset);
						break;
					case Tile::TileClass::stone:
						tile = std::make_unique<StoneTile>(temperature_offset);
						break;
					case Tile::TileClass::water:
						tile = std::make_unique<WaterTile>(temperature_offset);
						break;
					default:
						throw std::logic_error{"Unrecognized tile type."};
				}
			}
		}
	}
	
	void Section::save(char const* filename)
	{
		std::ofstream fout{filename};
		for (auto& r_row : _tiles) {
			for (auto& tile : r_row) {
				fout << static_cast<char>(tile->tile_class()) << ' ' << tile->temperature_offset << ' ';
			}
		}
	}

	std::optional<Id<Being>> Section::being_id(RegionTile::Point tile_coords) const
	{
		auto it = _being_map.find(tile_coords);
		return it != _being_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	std::optional<Id<Object>> Section::object_id(RegionTile::Point tile_coords) const
	{
		auto it = _object_map.find(tile_coords);
		return it != _object_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	void Section::add(Being& being)
	{
		being.section = this;
		auto result = _being_map.insert({being.coords, being.id});
		if (!result.second) {
			throw std::logic_error{"Attempted to place a being on top of another in a section."};
		}
	}

	void Section::add(Object& object)
	{
		object.section = this;
		auto result = _object_map.insert({object.coords, object.id});
		if (!result.second) {
			throw std::logic_error{"Attempted to place an object on top of another in a section."};
		}
	}

	typename void Section::remove_being(RegionTile::Point coords)
	{
		auto it = _being_map.find(coords);
		if (it != _being_map.end()) {
			if (Being* removed_being = game().beings.ptr(it->second)) {
				removed_being->section = nullptr;
			}
			_being_map.erase(it);
		}
	}

	typename void Section::remove_object(RegionTile::Point coords)
	{
		auto it = _object_map.find(coords);
		if (it != _object_map.end()) {
			if (Object* removed_object = game().objects.ptr(it->second)) {
				removed_object->section = nullptr;
			}
			_object_map.erase(it);
		}
	}

	void Section::remove(Being& being)
	{
		remove_being(being.coords);
	}

	void Section::remove(Object& object)
	{
		remove_object(object.coords);
	}

	void Section::add(LightSource const& light_source)
	{
		_light_source_ids.insert(light_source.id);
	}

	void Section::remove(LightSource const& light_source)
	{
		_light_source_ids.erase(light_source.id);
	}
}
