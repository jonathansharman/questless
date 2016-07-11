/**
* @file    Section.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Section class.
*/

#include "world/Section.h"

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

	HexCoords Section::section_coords(HexCoords region_coords)
	{
		int q = (region_coords.q % section_diameter + section_diameter) % section_diameter;
		int r = (region_coords.r % section_diameter + section_diameter) % section_diameter;
		return HexCoords{q, r};
	}

	Section::Section(HexCoords coords) : _coords{coords}
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
			for (auto& hex : r_row) {
				fout << static_cast<char>(hex->type());
			}
		}

		fout.close();
	}

	void Section::close()
	{
		for (auto& r_row : _tiles) {
			for (auto& hex : r_row) {
				hex = nullptr;
			}
		}
	}

	void Section::read(std::istream& in)
	{
		using std::make_unique;

		char c;
		for (auto& slice : _tiles) {
			for (auto& tile : slice) {
				in >> c;
				switch (static_cast<Tile::Type>(c)) {
					case Tile::Type::edge:
						tile = make_unique<EdgeTile>();
						break;
					case Tile::Type::stone:
						tile = make_unique<StoneTile>();
						break;
					case Tile::Type::dirt:
						tile = make_unique<DirtTile>();
						break;
					case Tile::Type::grass:
						tile = make_unique<GrassTile>();
						break;
					case Tile::Type::water:
						tile = make_unique<WaterTile>();
						break;
					case Tile::Type::snow:
						tile = make_unique<SnowTile>();
						break;
					default:
						throw std::logic_error{"Unrecognized tile type."};
				}
			}
		}
	}
}
