/**
* @file    Section.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Section class.
*/

#include "world/Section.h"

#include <fstream>

#include "Game.h"
#include "world/Region.h"

namespace questless
{
	SectionTileIndex Section::tile_index(SectionTileCoords tile_coords)
	{
		return SectionTileIndex
			{ tile_coords.r + Section::radius
			, tile_coords.q + Section::radius
			};
	}

	SectionTileIndex Section::tile_index(RegionTileCoords region_tile_coords)
	{
		return SectionTileIndex
			{ ((region_tile_coords.r + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
			, ((region_tile_coords.q + Section::radius) % Section::diameter + Section::diameter) % Section::diameter
			};
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
					case Tile::TileClass::edge:
						tile = std::make_unique<EdgeTile>(light_level, temperature);
						break;
					case Tile::TileClass::stone:
						tile = std::make_unique<StoneTile>(light_level, temperature);
						break;
					case Tile::TileClass::dirt:
						tile = std::make_unique<DirtTile>(light_level, temperature);
						break;
					case Tile::TileClass::grass:
						tile = std::make_unique<GrassTile>(light_level, temperature);
						break;
					case Tile::TileClass::water:
						tile = std::make_unique<WaterTile>(light_level, temperature);
						break;
					case Tile::TileClass::snow:
						tile = std::make_unique<SnowTile>(light_level, temperature);
						break;
					default:
						throw std::logic_error{"Unrecognized tile type."};
				}
			}
		}
	}
	
	void Section::save(std::string const& filename)
	{
		std::ofstream fout{filename.c_str()};
		for (auto& r_row : _tiles) {
			for (auto& tile : r_row) {
				fout << static_cast<char>(tile->tile_class()) << ' ' << tile->light_level() << ' ' << tile->temperature() << ' ';
			}
		}
	}

	std::vector<Being::ref> Section::beings() const
	{
		std::vector<Being::ref> beings;
		for (auto const& pair : _being_ids) {
			auto id = pair.second;
			beings.push_back(*game().beings[id]); // It is safe to assume anything referenced in the section still exists.
		}
		return beings;
	}

	std::vector<Object::ref> Section::objects() const
	{
		std::vector<Object::ref> objects;
		for (auto const& pair : _object_ids) {
			auto id = pair.second;
			objects.push_back(*game().objects[id]); // It is safe to assume anything referenced in the section still exists.
		}
		return objects;
	}

	boost::optional<Id<Being>> Section::being_id(RegionTileCoords tile_coords) const
	{
		auto it = _being_ids.find(tile_coords);
		return it != _being_ids.end() ? boost::make_optional(it->second) : boost::none;
	}

	boost::optional<Id<Object>> Section::object_id(RegionTileCoords tile_coords) const
	{
		auto it = _object_ids.find(tile_coords);
		return it != _object_ids.end() ? boost::make_optional(it->second) : boost::none;
	}

	typename void Section::remove_being(RegionTileCoords coords)
	{
		auto it = _being_ids.find(coords);
		if (it != _being_ids.end()) {
			if (Being* removed_being = game().beings[it->second]) {
				removed_being->section = nullptr;
			}
			_being_ids.erase(it);
		}
	}

	typename void Section::remove_object(RegionTileCoords coords)
	{
		auto it = _object_ids.find(coords);
		if (it != _object_ids.end()) {
			if (Object* removed_object = game().objects[it->second]) {
				removed_object->section = nullptr;
			}
			_object_ids.erase(it);
		}
	}
}
