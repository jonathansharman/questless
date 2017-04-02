/**
* @file    Region.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Region class.
*/

#include <filesystem>
namespace fs = std::tr2::sys; /// @todo Replace this with proper using statements if/when TR2 comes around.
#include <limits.h>
#include <sstream>
#include <fstream>

#include "Game.h"
#include "world/Region.h"
#include "agents/Agent.h"
#include "agents/LazyAI.h"
#include "agents/BasicAI.h"
#include "utility/utility.h"

#include "items/weapons/Quarterstaff.h"

using std::string;
using std::vector;
using std::function;
using std::unique_ptr;
using std::make_unique;

using namespace sdl;

namespace questless
{
	/// @todo Remove. Wait, why?
	bool turn_order_function(Being const& first, Being const& second)
	{
		// Sort beings in the turn queue by lower busy-time first, then lower entity ID.
		double f_b = first.busy_time;
		double s_b = second.busy_time;
		return f_b < s_b || (f_b == s_b && first.id < second.id);
	}

	Region::Region(string region_name)
		: _name{std::move(region_name)}, _turn_queue{turn_order_function}
	{
		int const r_radius = 1;
		int const q_radius = 1;

		for (int section_r = -r_radius; section_r <= r_radius; ++section_r) {
			for (int section_q = -q_radius; section_q <= q_radius; ++section_q) {
				RegionSectionCoords section_coords{section_q, section_r};

				// Create a section with random terrain.
				string data;
				for (int r = -Section::radius; r <= Section::radius; ++r) {
					for (int q = -Section::radius; q <= Section::radius; ++q) {
						if (r == Section::radius || q == Section::radius) {
							data += std::to_string(0) + ' ' + std::to_string(100.0) + ' ' + std::to_string(0.0) + ' ';
						} else {
							data += std::to_string(uniform(1, 5)) + ' ' + std::to_string(100.0) + ' ' + std::to_string(0.0) + ' ';
						}
					}
				}
				_section_map[section_coords] = make_unique<Section>(section_coords, std::istringstream{data});
				// Add beings randomly.
				for (int r = -Section::radius; r <= Section::radius; ++r) {
					for (int q = -Section::radius; q <= Section::radius; ++q) {
						if ((section_r != 0 || section_q != 0) && uniform(0, 10) == 0) {
							auto entity_coords = Section::region_tile_coords(section_coords, SectionTileCoords{q, r});
							auto new_being = make_unique<Goblin>(Agent::make<BasicAI>);
							new_being->inventory.add(game().items.add(make_unique<Quarterstaff>()).id);
							spawn(std::move(new_being), entity_coords);
						}
					}
				}
			}
		}
	}

	Region::Region(char const* save_name, string region_name)
		: _name{std::move(region_name)}, _turn_queue{turn_order_function}
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
			int section_q = std::stoi(q_string);
			int section_r = std::stoi(r_string);
			RegionSectionCoords section_coords{section_q, section_r};

			std::ifstream data_stream{(region_path / it->path()).string()};
			if (data_stream.fail()) {
				throw std::logic_error("Could not open section file.");
			}
			_section_map[section_coords] = make_unique<Section>(section_coords, data_stream);
		}

		fs::path entities_filename{"entities"};
		std::ifstream fin;
		fin.open((region_path / entities_filename).string());
		if (fin.fail()) {
			throw std::logic_error("Could not open region's entities file.");
		}
		string line;
		while (std::getline(fin, line)) {
			std::istringstream sin(line, std::ios::hexfloat);

			unsigned entity_id;
			sin >> entity_id;
			switch (static_cast<EntityClass>(entity_id)) {
				case EntityClass::GoblinClass:
				{
					Being& goblin = game().beings.add(make_unique<Goblin>(sin));
					add(goblin, goblin.coords);
					break;
				}
				case EntityClass::TrollClass:
					break;
				default:
					throw std::logic_error("Unknown entity type in entities file.");
			}
		}
	}

	void Region::save(char const* save_name)
	{
		/// @todo Reenable someday. Use SQLite or something to save games.

#if 0
		/// @todo How do deal with deleted sections? Their files should be cleaned out somehow.

		fs::path saves_dir{"saves"};
		fs::path save_filename{save_name};
		fs::path regions_dir{"regions"};
		fs::path region_filename{_name};
		fs::path region_path{saves_dir / save_filename / regions_dir / region_filename};

		for (auto const& coords_and_section : _section_map) {
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
				throw std::logic_error("Could not open region's entities file.");
			}
			for (auto const& coords_and_section : _section_map) {
				auto const& section = coords_and_section.second;

				for (auto const& being : section->beings()) {
					being->serialize(fout);
				}
				for (auto const& object : section->objects()) {
					object->serialize(fout);
				}
			}
		}
#endif
	}

	Being* Region::next_ready_being()
	{
		if (_turn_queue.empty()) {
			return nullptr;
		} else {
			Being& next = *_turn_queue.begin();
			return &next;
		}
	}

	std::optional<Id<Being>> Region::being_id(RegionTileCoords tile_coords) const
	{
		RegionSectionCoords section_coords = containing_section_coords(tile_coords);
		auto it = _section_map.find(section_coords);
		return it != _section_map.end() ? it->second->being_id(tile_coords) : std::nullopt;
	}

	std::optional<Id<Object>> Region::object_id(RegionTileCoords tile_coords) const
	{
		RegionSectionCoords section_coords = containing_section_coords(tile_coords);
		auto it = _section_map.find(section_coords);
		return it != _section_map.end() ? it->second->object_id(tile_coords) : std::nullopt;
	}


	Being* Region::being(RegionTileCoords tile_coords) const
	{
		if (auto opt_id = being_id(tile_coords)) {
			return game().beings.get(*opt_id);
		} else {
			return nullptr;
		}
	}

	Object* Region::object(RegionTileCoords tile_coords) const
	{
		if (auto opt_id = object_id(tile_coords)) {
			return game().objects.get(*opt_id);
		} else {
			return nullptr;
		}
	}

	void Region::spawn_player(Being::uptr player_being)
	{
		/// @todo More advanced player spawning.

		// Put the player's character somewhere in the middle section.

		int q = uniform(-Section::radius, Section::radius);
		int r = uniform(-Section::radius, Section::radius);
		RegionTileCoords player_coords{q, r};

		// Erase the being currently there, if any.
		auto& section = containing_section(player_coords);
		section.remove_being(player_coords);

		spawn(std::move(player_being), player_coords);
	}

	void Region::add(Being& being, RegionTileCoords coords)
	{
		RegionSectionCoords section_coords = containing_section_coords(coords);
		auto it = _section_map.find(section_coords);
		if (it != _section_map.end()) {
			Section* section = it->second.get();
			being.region = this;
			being.section = section;
			being.coords = coords;

			add_to_turn_queue(being);
			section->add(being);
		}
	}

	void Region::add(Object& object, RegionTileCoords coords)
	{
		RegionSectionCoords section_coords = containing_section_coords(coords);
		auto it = _section_map.find(section_coords);
		if (it != _section_map.end()) {
			Section* section = it->second.get();
			object.region = this;
			object.section = section;
			object.coords = coords;

			section->add(object);
		}
	}

	void Region::spawn(Being::uptr being, RegionTileCoords coords)
	{
		add(game().beings.add(std::move(being)), coords);
	}

	void Region::spawn(Object::uptr object, RegionTileCoords coords)
	{
		add(game().objects.add(std::move(object)), coords);
	}

	bool Region::move(Being& being, RegionTileCoords coords)
	{
		Section& src_section = *being.section;
		RegionSectionCoords dst_section_coords = containing_section_coords(coords);
		if (dst_section_coords != src_section.coords()) {
			unique_ptr<Section> const& dst_section = _section_map[dst_section_coords];
			if (dst_section != nullptr) {
				if (dst_section->being_id(coords)) {
					// Collision with another being. Prevent movement.
					return false;
				}
				if (auto object_id = dst_section->object_id(coords)) {
					if (game().objects.get(*object_id)->blocks_movement()) {
						// Collision with an obstructing object. Prevent movement.
						return false;
					}
				}

				src_section.remove(being);
				being.coords = coords;
				dst_section->add(being);
			} else {
				/// @todo Need to deal with null destination case.
				return false;
			}
		} else {
			if (src_section.being_id(coords)) {
				// Collision with another being. Prevent movement.
				return false;
			}
			if (auto object_id = src_section.object_id(coords)) {
				if (game().objects.get(*object_id)->blocks_movement()) {
					// Collision with an obstructing object. Prevent movement.
					return false;
				}
			}
			src_section.remove(being);
			being.coords = coords;
			src_section.add(being);
		}
		return true;
	}

	bool Region::move(Object& object, RegionTileCoords coords)
	{
		Section& src_section = *object.section;
		RegionSectionCoords dst_section_coords = containing_section_coords(coords);
		if (dst_section_coords != src_section.coords()) {
			unique_ptr<Section> const& dst_section = _section_map[dst_section_coords];
			if (dst_section != nullptr) {
				if (dst_section->being_id(coords) || dst_section->object_id(coords)) {
					// Collision. Prevent movement.
					return false;
				}
				src_section.remove(object);
				object.coords = coords;
				dst_section->add(object);
			} else {
				/// @todo Need to deal with null destination case.
				return false;
			}
		} else {
			if (src_section.object_id(coords)) {
				// Collision. Prevent movement.
				return false;
			}
			src_section.remove(object);
			object.coords = coords;
			src_section.add(object);
		}
		return true;
	}
	
	void Region::remove(Being& being)
	{
		Section& section = *being.section;

		being.region = nullptr;
		being.section = nullptr;

		remove_from_turn_queue(being);
		section.remove(being);
	}

	void Region::remove(Object& object)
	{
		Section& section = *object.section;

		object.region = nullptr;
		object.section = nullptr;

		section.remove(object);
	}

	bool Region::section_exists(RegionSectionCoords section_coords) const
	{
		auto it = _section_map.find(section_coords);
		return it != _section_map.end() && it->second != nullptr;
	}

	Section* Region::section(RegionSectionCoords section_coords)
	{
		auto it = _section_map.find(section_coords);
		return it == _section_map.end() ? nullptr : it->second.get();
	}
	Section const* Region::section(RegionSectionCoords section_coords) const
	{
		auto it = _section_map.find(section_coords);
		return it == _section_map.end() ? nullptr : it->second.get();
	}

	void Region::update()
	{
		std::vector<Id<Being>> beings_to_update;
		std::vector<Id<Object>> objects_to_update;
		for_each_loaded_section([&](Section& section) {
			for (Being& being : section.beings()) {
				beings_to_update.push_back(being.id);
			}
			for (Object& object : section.objects()) {
				objects_to_update.push_back(object.id);
			}
		});
		for (Id<Being> being_id : beings_to_update) {
			if (Being* being = game().beings.get(being_id)) {
				being->update();
			}
		}
		for (Id<Object> object_id : objects_to_update) {
			if (Object* object = game().objects.get(object_id)) {
				object->update();
			}
		}
	}

	void Region::for_each_loaded_section(function<void(Section&)> f)
	{
		for (int r = -_loaded_sections_q_radius; r <= _loaded_sections_q_radius; ++r) {
			for (int q = -_loaded_sections_r_radius; q <= _loaded_sections_r_radius; ++q) {
				RegionSectionCoords section_coords{q, r};
				auto it = _section_map.find(section_coords);
				if (it != _section_map.end() && it->second != nullptr) { /// @todo Null sections...?
					f(*it->second);
				}
			}
		}
	}
}
