/**
* @file    Section.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Section class.
*/

#include "world/Section.h"
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
		int x = tile_coords.hex.r + section_radius;
		int y = tile_coords.hex.q + section_radius;
		return SectionTileIndex{x, y};
	}

	SectionTileIndex Section::tile_index(RegionTileCoords region_tile_coords)
	{
		int x = (region_tile_coords.hex.q % section_diameter + section_diameter) % section_diameter;
		int y = (region_tile_coords.hex.r % section_diameter + section_diameter) % section_diameter;
		return SectionTileIndex{x, y};
	}

	Section::Section(RegionSectionCoords coords) : _coords{coords}
	{}

	void Section::create(const string& data)
	{
		std::istringstream sin(data);
		read(sin);
	}

	void Section::open(const string& filename)
	{
		std::ifstream fin(filename.c_str());
		if (fin.fail()) {
			throw logic_error("Could not open section file.");
		}
		read(fin);
		fin.close();
	}
	
	void Section::save(const string& filename)
	{
		std::ofstream fout(filename.c_str());

		for (auto& r_row : _tiles) {
			for (auto& tile : r_row) {
				fout << static_cast<char>(tile->tile_class()) << ' ' << tile->light_level() << ' ' << tile->temperature() << ' ';
			}
		}

		fout.close();
	}

	void Section::close()
	{
		for (auto& r_row : _tiles) {
			for (auto& tile : r_row) {
				tile = nullptr;
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

	void Section::read(std::istream& in)
	{
		using std::make_unique;

		for (auto& slice : _tiles) {
			for (auto& tile : slice) {
				int c;
				double light_level;
				double temperature;

				in >> c >> light_level >> temperature;

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
}
