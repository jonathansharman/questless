/**
* @file    Region.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Region class.
*/

#include <limits.h>

#include "world/Region.h"
#include "entities/beings/Agent.h"
#include "entities/beings/LazyAI.h"
#include "utility/utility.h"

using std::string;
using std::vector;
using std::function;
using std::unique_ptr;

using namespace sdl;

namespace questless
{
	/// @todo Remove.
	bool turn_order_function(const Being& first, const Being& second)
	{
		// Sort beings in the turn queue by lower busy-time first, then lower entity ID.
		double f_b = first.busy_time();
		double s_b = second.busy_time();
		return f_b < s_b || (f_b == s_b && first.entity_id() < second.entity_id());
	}

	Region::Region(string region_name) : _name{std::move(region_name)}, _turn_queue{turn_order_function}
	{
		const int r_radius = 1;
		const int q_radius = 1;

		for (int section_r = -r_radius; section_r <= r_radius; ++section_r) {
			for (int section_q = -q_radius; section_q <= q_radius; ++section_q) {
				if (uniform(0, 0)) {
				} else {
					HexCoords section_coords{section_q, section_r};
					_section_map[section_coords] = make_unique<Section>(section_coords);

					// Create a random section.
					string data;
					for (int r = -section_radius; r <= section_radius; ++r) {
						for (int q = -section_radius; q <= section_radius; ++q) {
#if 1 // Tiles based on coords
							if (r == section_radius) {
								data += char(0);
							} else {
								if (q == section_radius) {
									data += char(0);
								} else {
									data += char(uniform(1, 5));
									if ((section_r != 0 || section_q != 0) && uniform(0, 10) == 0) {
										HexCoords entity_coords = HexCoords{q, r} +section_coords * section_diameter;
										auto new_being = make_unique<Goblin>(Agent::make<LazyAI>, Entity::next_id());
										add<Being>(std::move(new_being), entity_coords);
									}
								}
							}
#else // Random tiles
							char new_tile = static_cast<char>(uniform(0, 5));
							data += new_tile;
							if (new_tile != 0) {
								if (section_r != 0 && section_q != 0 && uniform(0, 10) == 0) {
									HexCoords entity_coords = HexCoords{q, r} + section_coords * section_diameter;
									auto new_being = make_shared<Goblin>(Agent::make<LazyAI>, _name, entity_coords);
									add_entity(new_being->as_entity());
								}
							}
#endif
						}
					}
					_section_map[section_coords]->create(data);
				}
			}
		}
	}

	Region::Region(const string& save_name, const string& region_name) : _name{std::move(region_name)}, _turn_queue{turn_order_function}
	{
		fs::path saves_dir{"saves"};
		fs::path save_filename{save_name};
		fs::path regions_dir{"regions"};
		fs::path region_filename{_name};
		fs::path region_path = saves_dir / save_filename / regions_dir / region_filename;

		for (fs::directory_iterator it(region_path); it != fs::directory_iterator(); ++it) {
			string section_stem = it->path().stem().string();
			string q_string, r_string;
			bool reading_q = true;
			for (char c : section_stem) {
				if (c == '_') {
					reading_q = false;
				} else if (reading_q) {
					q_string += c;
				} else {
					r_string += c;
				}
			}
			int q = std::stoi(q_string);
			int r = std::stoi(r_string);
			HexCoords section_coords{q, r};

			_section_map[section_coords]->open((region_path / it->path()).string());
		}

		fs::path entities_filename{"entities"};
		std::ifstream fin;
		fin.open((region_path / entities_filename).string());
		if (fin.fail()) {
			throw logic_error("Could not open region's entities file.");
		}
		string line;
		while (std::getline(fin, line)) {
			std::istringstream sin(line, std::ios::hexfloat);

			unsigned entity_id;
			sin >> entity_id;
			switch (static_cast<EntityClass>(entity_id)) {
				case EntityClass::GoblinClass:
					add<Being>(make_unique<Goblin>(sin)); /// @todo How will this handle references to other entities?
					break;
				case EntityClass::TrollClass:
					break;
				default:
					throw logic_error("Unknown entity type in entities file.");
			}
		}
	}

	void Region::save(const string& save_name)
	{
		return; /// @todo Reenable someday. Use SQLite or something to save games.

		/// @todo How do deal with deleted sections? Their files should be cleaned out somehow.

		fs::path saves_dir{"saves"};
		fs::path save_filename{save_name};
		fs::path regions_dir{"regions"};
		fs::path region_filename{_name};
		fs::path region_path{saves_dir / save_filename / regions_dir / region_filename};

		for (const auto& coords_and_section : _section_map) {
			HexCoords coords = coords_and_section.first;

			std::ostringstream section_filename_sstream;
			section_filename_sstream << coords.q << '_' << coords.r << ".hex";
			fs::path hex_filename{section_filename_sstream.str()};

			_section_map[coords]->save((region_path / hex_filename).string());
		}

		fs::path entities_filename{"entities.dat"};
		{
			std::ofstream fout{(region_path / entities_filename).string()};
			if (fout.fail()) {
				throw logic_error("Could not open region's entities file.");
			}
			for (const auto& coords_and_section : _section_map) {
				const auto& section = coords_and_section.second;

				for (const auto& being : section->beings()) {
					being->serialize(fout);
				}
				for (const auto& object : section->objects()) {
					object->serialize(fout);
				}
			}
		}
	}

	Being* Region::next_ready_being()
	{
		if (_turn_queue.empty()) {
			return nullptr;
		} else {
			Being& next = *_turn_queue.begin();
			return &next;
		}

		/// @todo Put this in a "wandering" AI.
		//switch (uniform(0, 5)) {
		//	case 0:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::one));
		//		break;
		//	case 1:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::two));
		//		break;
		//	case 2:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::three));
		//		break;
		//	case 3:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::four));
		//		break;
		//	case 4:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::five));
		//		break;
		//	case 5:
		//		move_entity(being->as_entity(), being->coords().neighbor(HexCoords::Direction::six));
		//		break;
		//}
	}

	void Region::spawn_player(Being::ptr player_being)
	{
		/// @todo More advanced player spawning.

		// Put the player's character somewhere in the middle section.

		int q = uniform(-section_radius, section_radius);
		int r = uniform(-section_radius, section_radius);
		HexCoords player_coords = HexCoords{q, r};

		// Erase the being currently there, if any.
		auto& section = _section_map[containing_section_coords(player_coords)];
		section->remove<Being>([=](const Being::ptr& being) { return being->coords() == player_coords; });

		add<Being>(std::move(player_being), player_coords);
	}

	void Region::draw_terrain(const Camera& camera)
	{
		camera.draw(*_background, Point{0, 0});
	}

	void Region::cache_background(sdl::Renderer& renderer)
	{
		HexCoords most_distant_hex_coords{-(_loaded_sections_q_radius + 1) * section_diameter, -(_loaded_sections_r_radius + 1) * section_diameter};
		Point most_distant_world_coords = Layout::dflt().to_world(most_distant_hex_coords);

		_background = make_unique<Texture>
			( renderer
			, SDL_BLENDMODE_BLEND
			, 2 * abs(most_distant_world_coords.x) + 1
			, 2 * abs(most_distant_world_coords.y) + 1
			, true
			);
		Vector center{_background->width() / 2, _background->height() / 2};
		_background->as_target([&] {
			for (unsigned i = 0; i < 20; ++i) {
				renderer.draw_rect(Rect(i, i, _background->width() - 2 * i, _background->height() - 2 * i), Color::red(), false);
			}
			for_each_loaded_section([center](Section& section) {
//				section.draw(center);
			});
		});
	}

	Entity* Region::entity(HexCoords tile_coords) const
	{
		HexCoords section_coords = containing_section_coords(tile_coords);
		if (section_exists(section_coords)) {
			const Section& s = section(section_coords);
			for (const auto& being : s.beings()) {
				if (being->coords() == tile_coords) {
					return &being->as_entity();
				}
			}
			for (const auto& object : s.objects()) {
				if (object->coords() == tile_coords) {
					return &object->as_entity();
				}
			}
		}
		return nullptr;
	}
	
	bool Region::section_exists(HexCoords section_coords) const
	{
		auto it = _section_map.find(section_coords);
		return it != _section_map.end() && it->second != nullptr;
	}

	const Section& Region::section(HexCoords section_coords) const
	{
		auto it = _section_map.find(section_coords);
		if (it != _section_map.end()) {
			return *(it->second);
		} else {
			throw logic_error{"No section at given coordinates in region."};
		}
	}

	void Region::update()
	{
		for_each_loaded_section([](Section& section) {
			for (const auto& being : section.beings()) {
				being->update();
			}
			for (const auto& object : section.objects()) {
				object->update();
			}
		});
	}

	void Region::for_each_loaded_section(function<void(Section&)> f)
	{
		for (int r = -_loaded_sections_q_radius; r <= _loaded_sections_q_radius; ++r) {
			for (int q = -_loaded_sections_r_radius; q <= _loaded_sections_r_radius; ++q) {
				HexCoords section_coords{q, r};
				auto it = _section_map.find(section_coords);
				if (it != _section_map.end() && it->second != nullptr) { /// @todo Null sections...?
					f(*it->second);
				}
			}
		}
	}
}
