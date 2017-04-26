//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/Section.h"

#include <fstream>

#include "Game.h"
#include "world/LightSource.h"
#include "world/Region.h"

namespace questless
{
	RegionSectionCoords Section::region_section_coords(RegionTileCoords region_tile_coords)
	{
		int q = region_tile_coords.q >= 0
			? (region_tile_coords.q + Section::radius) / Section::diameter
			: (region_tile_coords.q - Section::radius) / Section::diameter
			;
		int r = region_tile_coords.r >= 0
			? (region_tile_coords.r + Section::radius) / Section::diameter
			: (region_tile_coords.r - Section::radius) / Section::diameter
			;
		return RegionSectionCoords{q, r};
	}

	Section::Section(RegionSectionCoords coords, std::istream& data_stream) : _coords{coords}
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
	
	std::vector<cref<Being>> Section::beings() const
	{
		std::vector<cref<Being>> beings;
		for (auto const& pair : _being_ids) {
			auto id = pair.second;
			beings.push_back(game().beings.get_ref(id));
		}
		return beings;
	}
	std::vector<ref<Being>> Section::beings()
	{
		std::vector<ref<Being>> beings;
		for (auto const& pair : _being_ids) {
			auto id = pair.second;
			beings.push_back(game().beings.get_ref(id));
		}
		return beings;
	}
	
	std::vector<cref<Object>> Section::objects() const
	{
		std::vector<cref<Object>> objects;
		for (auto const& pair : _object_ids) {
			auto id = pair.second;
			objects.push_back(game().objects.get_ref(id));
		}
		return objects;
	}
	std::vector<ref<Object>> Section::objects()
	{
		std::vector<ref<Object>> objects;
		for (auto const& pair : _object_ids) {
			auto id = pair.second;
			objects.push_back(game().objects.get_ref(id));
		}
		return objects;
	}

	std::optional<Id<Being>> Section::being_id(RegionTileCoords tile_coords) const
	{
		auto it = _being_ids.find(tile_coords);
		return it != _being_ids.end() ? std::make_optional(it->second) : std::nullopt;
	}

	std::optional<Id<Object>> Section::object_id(RegionTileCoords tile_coords) const
	{
		auto it = _object_ids.find(tile_coords);
		return it != _object_ids.end() ? std::make_optional(it->second) : std::nullopt;
	}

	typename void Section::remove_being(RegionTileCoords coords)
	{
		auto it = _being_ids.find(coords);
		if (it != _being_ids.end()) {
			if (Being* removed_being = game().beings.get(it->second)) {
				removed_being->section = nullptr;
			}
			_being_ids.erase(it);
		}
	}

	typename void Section::remove_object(RegionTileCoords coords)
	{
		auto it = _object_ids.find(coords);
		if (it != _object_ids.end()) {
			if (Object* removed_object = game().objects.get(it->second)) {
				removed_object->section = nullptr;
			}
			_object_ids.erase(it);
		}
	}
	
	std::vector<cref<LightSource>> Section::light_sources() const
	{
		//! @todo Could make a lazy version of this, as in Inventory::Item. (Eager evaluation should be okay here though since the number of light sources will be small.)
		//! @todo Make a generic List<Id<T>> -> List<T&> iterator/container type?

		std::vector<cref<LightSource>> result;
		for (Id<LightSource> id : _light_source_ids) {
			result.push_back(game().light_sources.get_ref(id));
		}
		return result;
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
