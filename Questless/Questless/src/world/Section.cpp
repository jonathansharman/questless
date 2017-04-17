//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/Section.h"

#include <fstream>

#include "Game.h"
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
				double light_level;
				double temperature;

				data_stream >> c >> light_level >> temperature;

				switch (static_cast<Tile::TileClass>(c)) {
					case Tile::TileClass::dirt:
						tile = std::make_unique<DirtTile>(light_level, temperature);
						break;
					case Tile::TileClass::edge:
						tile = std::make_unique<EdgeTile>(light_level, temperature);
						break;
					case Tile::TileClass::grass:
						tile = std::make_unique<GrassTile>(light_level, temperature);
						break;
					case Tile::TileClass::sand:
						tile = std::make_unique<SandTile>(light_level, temperature);
						break;
					case Tile::TileClass::snow:
						tile = std::make_unique<SnowTile>(light_level, temperature);
						break;
					case Tile::TileClass::stone:
						tile = std::make_unique<StoneTile>(light_level, temperature);
						break;
					case Tile::TileClass::water:
						tile = std::make_unique<WaterTile>(light_level, temperature);
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
				fout << static_cast<char>(tile->tile_class()) << ' ' << tile->light_level() << ' ' << tile->temperature() << ' ';
			}
		}
	}
	
	std::vector<Being::cref> Section::beings() const
	{
		std::vector<Being::cref> beings;
		for (auto const& pair : _being_ids) {
			auto id = pair.second;
			beings.push_back(game().beings.get_ref(id));
		}
		return beings;
	}
	std::vector<Being::ref> Section::beings()
	{
		std::vector<Being::ref> beings;
		for (auto const& pair : _being_ids) {
			auto id = pair.second;
			beings.push_back(game().beings.get_ref(id));
		}
		return beings;
	}
	
	std::vector<Object::cref> Section::objects() const
	{
		std::vector<Object::cref> objects;
		for (auto const& pair : _object_ids) {
			auto id = pair.second;
			objects.push_back(game().objects.get_ref(id));
		}
		return objects;
	}
	std::vector<Object::ref> Section::objects()
	{
		std::vector<Object::ref> objects;
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
}
