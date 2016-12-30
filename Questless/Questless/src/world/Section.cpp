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

#include "world/Region.h"

namespace questless
{
	/// @todo Remove if still not used anywhere later.
	//bool being_comparison_function(const Being::ptr& first, const Being::ptr& second)
	//{
	//	// Sort beings by lower y-coordinate first (so beings can be quickly drawn front-to-back), then lower entity ID.
	//	// This is a safe key to use because whenever a being's coordinates change, it is removed and reinserted with the new coordinates.
	//	int f_y = Layout::dflt().to_world(first->coords()).y;
	//	int s_y = Layout::dflt().to_world(second->coords()).y;
	//	return f_y < s_y || (f_y == s_y && first->entity_id() < second->entity_id());
	//}

	SectionTileIndex Section::tile_index(SectionTileCoords tile_coords)
	{
		int x = tile_coords.r + Section::radius;
		int y = tile_coords.q + Section::radius;
		return SectionTileIndex{x, y};
	}

	SectionTileIndex Section::tile_index(RegionTileCoords region_tile_coords)
	{
		int x = (region_tile_coords.q % Section::diameter + Section::diameter) % Section::diameter;
		int y = (region_tile_coords.r % Section::diameter + Section::diameter) % Section::diameter;
		return SectionTileIndex{x, y};
	}

	Section::Section(RegionSectionCoords coords, std::istream& data_stream) : _coords{coords}
	{
		using std::make_unique;
		for (auto& slice : _tiles) {
			for (auto& tile : slice) {
				int c;
				double light_level;
				double temperature;

				data_stream >> c >> light_level >> temperature;

				switch (static_cast<Tile::TileClass>(c)) {
					case Tile::TileClass::edge:
						tile = make_unique<EdgeTile>(light_level, temperature);
						break;
					case Tile::TileClass::stone:
						tile = make_unique<StoneTile>(light_level, temperature);
						break;
					case Tile::TileClass::dirt:
						tile = make_unique<DirtTile>(light_level, temperature);
						break;
					case Tile::TileClass::grass:
						tile = make_unique<GrassTile>(light_level, temperature);
						break;
					case Tile::TileClass::water:
						tile = make_unique<WaterTile>(light_level, temperature);
						break;
					case Tile::TileClass::snow:
						tile = make_unique<SnowTile>(light_level, temperature);
						break;
					default:
						throw std::logic_error{"Unrecognized tile type."};
				}
			}
		}
	}
	
	void Section::save(const std::string& filename)
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
		for (const auto& pair : _beings) {
			beings.push_back(*pair.second);
		}
		return beings;
	}

	std::vector<Object::ref> Section::objects() const
	{
		std::vector<Object::ref> objects;
		for (const auto& pair : _objects) {
			objects.push_back(*pair.second);
		}
		return objects;
	}

	Being* Section::being(RegionTileCoords tile_coords) const
	{
		auto it = _beings.find(tile_coords);
		return it == _beings.end() ? nullptr : it->second.get();
	}

	Object* Section::object(RegionTileCoords tile_coords) const
	{
		auto it = _objects.find(tile_coords);
		return it == _objects.end() ? nullptr : it->second.get();
	}
}
