//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <filesystem>
namespace fs = std::experimental::filesystem; //! @todo Replace this with std::filesystem when available.
#include <limits.h>
#include <sstream>
#include <fstream>

#include "agents/Agent.h"
#include "agents/LazyAI.h"
#include "agents/BasicAI.h"
#include "entities/all-entities.h"
#include "Game.h"
#include "items/weapons/Quarterstaff.h"
#include "units/constants.h"
#include "utility/utility.h"
#include "world/LightSource.h"
#include "world/Region.h"

using std::string;
using std::vector;
using std::function;
using std::make_unique;

using namespace sdl;

namespace questless
{
	bool turn_order_function(Being const& first, Being const& second)
	{
		// Sort beings in the turn queue by lower busy-time first, then lower entity ID.
		double f_b = first.busy_time;
		double s_b = second.busy_time;
		return f_b < s_b || (f_b == s_b && first.id < second.id);
	}

	Region::Region(string region_name)
		: _name{std::move(region_name)}
		, _time{0.0}
		, _turn_queue{turn_order_function}
		, _ambient_illuminance{get_ambient_illuminance()}
	{
		int const r_radius = 1;
		int const q_radius = 1;

		for (int section_r = -r_radius; section_r <= r_radius; ++section_r) {
			for (int section_q = -q_radius; section_q <= q_radius; ++section_q) {
				RegionSection::Point section_coords{section_q, section_r};

				// Create a section with random terrain.
				string data;
				for (int q = 0; q < Section::diameter; ++q) {
					for (int r = 0; r < Section::diameter; ++r) {
						//if (r == Section::diameter - 1 || q == Section::diameter - 1) {
						//	data += std::to_string(static_cast<int>(Tile::TileClass::edge)) + ' ' + std::to_string(0.0) + ' ';
						//} else {
							data += std::to_string(uniform(0, static_cast<int>(Tile::TileClass::TILE_CLASS_COUNT) - 1)) + ' ' + std::to_string(0.0) + ' ';
						//}

						// Every tile is snow (nice for testing lighting).
						//data += std::to_string(static_cast<int>(Tile::TileClass::snow)) + " 0 ";
					}
				}
				std::istringstream data_stream{data};
				_section_map.insert(std::make_pair(section_coords, Section{section_coords, data_stream}));
				// Add beings randomly.
				for (int q = 0; q < Section::diameter; ++q) {
					for (int r = 0; r < Section::diameter; ++r) {
						if ((section_r != 0 || section_q != 0) && uniform(0, 10) == 0) {
							auto entity_coords = Section::region_tile_coords(section_coords, SectionTile::Point{q, r});
							if (!uniform(0, 12)) {
								spawn(make_unique<Campfire>(*this, entity_coords), entity_coords);
							} else {
								auto new_being = make_unique<Goblin>(Agent::make<BasicAI>);
								new_being->inventory.add(game().items.add(make_unique<Quarterstaff>()).id);
								spawn(std::move(new_being), entity_coords);
							}
						}
					}
				}
			}
		}
	}

	Region::Region(char const* save_name, string region_name)
		: _name{std::move(region_name)}
		, _time{0.0}
		, _turn_queue{turn_order_function}
		, _ambient_illuminance{get_ambient_illuminance()}
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
			RegionSection::Point section_coords{section_q, section_r};

			std::ifstream data_stream{(region_path / it->path()).string()};
			if (data_stream.fail()) {
				throw std::logic_error("Could not open section file.");
			}
			_section_map.insert(std::make_pair(section_coords, Section{section_coords, data_stream}));
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

	void Region::save(char const* /*save_name*/)
	{
		//! @todo Reenable someday. Use SQLite or something to save games.

#if 0
		//! @todo How do deal with deleted sections? Their files should be cleaned out somehow.

		fs::path saves_dir{"saves"};
		fs::path save_filename{save_name};
		fs::path regions_dir{"regions"};
		fs::path region_filename{_name};
		fs::path region_path{saves_dir / save_filename / regions_dir / region_filename};

		for (auto const& coords_and_section : _section_map) {
			HexCoords region_tile_coords = coords_and_section.first;

			std::ostringstream section_filename_sstream;
			section_filename_sstream << region_tile_coords.q << '_' << region_tile_coords.r << ".hex";
			fs::path hex_filename{section_filename_sstream.str()};

			_section_map[region_tile_coords]->save((region_path / hex_filename).string());
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

	std::optional<Id<Being>> Region::being_id(RegionTile::Point region_tile_coords) const
	{
		if (Section const* section = containing_section(region_tile_coords)) {
			return section->being_id(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	std::optional<Id<Object>> Region::object_id(RegionTile::Point region_tile_coords) const
	{
		if (Section const* section = containing_section(region_tile_coords)) {
			return section->object_id(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	void Region::spawn_player(uptr<Being> player_being)
	{
		//! @todo More advanced player spawning.

		// Put the player's character somewhere in the middle section.

		int q = uniform(-Section::radius, Section::radius);
		int r = uniform(-Section::radius, Section::radius);
		RegionTile::Point player_coords{q, r};

		// Erase the being currently there, if any.
		Section* section = containing_section(player_coords);
		section->remove_being(player_coords);

		spawn(std::move(player_being), player_coords);
	}

	void Region::add(Being& being, RegionTile::Point region_tile_coords)
	{
		if (Section* section = containing_section(region_tile_coords)) {
			being.region = this;
			being.section = section;
			being.coords = region_tile_coords;

			add_to_turn_queue(being);
			section->add(being);
		} else {
			//! @todo What to do when adding outside current sections?
		}
	}

	void Region::add(Object& object, RegionTile::Point region_tile_coords)
	{
		if (Section* section = containing_section(region_tile_coords)) {
			object.region = this;
			object.section = section;
			object.coords = region_tile_coords;

			section->add(object);
		} else {
			//! @todo What to do when adding outside current sections?
		}
	}

	void Region::spawn(uptr<Being> being, RegionTile::Point region_tile_coords)
	{
		add(game().beings.add(std::move(being)), region_tile_coords);
	}

	void Region::spawn(uptr<Object> object, RegionTile::Point region_tile_coords)
	{
		add(game().objects.add(std::move(object)), region_tile_coords);
	}

	bool Region::move(Being& being, RegionTile::Point region_tile_coords)
	{
		Section& src_section = *being.section;
		Section* dst_section = containing_section(region_tile_coords);
		if (dst_section != &src_section) {
			if (dst_section != nullptr) {
				if (dst_section->being_id(region_tile_coords)) {
					// Collision with another being. Prevent movement.
					return false;
				}
				if (auto object_id = dst_section->object_id(region_tile_coords)) {
					if (game().objects.get(*object_id)->blocks_movement()) {
						// Collision with an obstructing object. Prevent movement.
						return false;
					}
				}

				src_section.remove(being);
				being.coords = region_tile_coords;
				dst_section->add(being);
			} else {
				//! @todo Need to deal with null destination case.
				return false;
			}
		} else {
			if (src_section.being_id(region_tile_coords)) {
				// Collision with another being. Prevent movement.
				return false;
			}
			if (auto object_id = src_section.object_id(region_tile_coords)) {
				if (game().objects.get(*object_id)->blocks_movement()) {
					// Collision with an obstructing object. Prevent movement.
					return false;
				}
			}
			src_section.remove(being);
			being.coords = region_tile_coords;
			src_section.add(being);
		}
		return true;
	}

	bool Region::move(Object& object, RegionTile::Point region_tile_coords)
	{
		Section& src_section = *object.section;
		Section* dst_section = containing_section(region_tile_coords);
		if (dst_section != &src_section) {
			if (dst_section != nullptr) {
				if (dst_section->being_id(region_tile_coords) || dst_section->object_id(region_tile_coords)) {
					// Collision. Prevent movement.
					return false;
				}
				src_section.remove(object);
				object.coords = region_tile_coords;
				dst_section->add(object);
			} else {
				//! @todo Need to deal with null destination case.
				return false;
			}
		} else {
			if (src_section.object_id(region_tile_coords)) {
				// Collision. Prevent movement.
				return false;
			}
			src_section.remove(object);
			object.coords = region_tile_coords;
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

	void Region::add(LightSource const& light_source)
	{
		int range = light_source.range();
		int const min_q = light_source.coords().q - range;
		int const min_r = light_source.coords().r - range;
		RegionTile::Point const min_region_tile_coords{min_q, min_r};
		RegionSection::Point const min_section_coords = Section::region_section_coords(min_region_tile_coords);

		int const max_q = light_source.coords().q + range;
		int const max_r = light_source.coords().r + range;
		RegionTile::Point const max_region_tile_coords{max_q, max_r};
		RegionSection::Point const max_section_coords = Section::region_section_coords(max_region_tile_coords);

		for (int section_q = min_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (int section_r = min_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				RegionSection::Point section_coords{section_q, section_r};
				if (Section* s = section(section_coords)) {
					s->add(light_source);
				}
			}
		}
	}

	void Region::remove(LightSource const& light_source)
	{
		int const min_q = light_source.coords().q - light_source.range();
		int const min_r = light_source.coords().r - light_source.range();
		RegionTile::Point const min_region_tile_coords{min_q, min_r};
		RegionSection::Point const min_section_coords = Section::region_section_coords(min_region_tile_coords);

		int const max_q = light_source.coords().q + light_source.range();
		int const max_r = light_source.coords().r + light_source.range();
		RegionTile::Point const max_region_tile_coords{max_q, max_r};
		RegionSection::Point const max_section_coords = Section::region_section_coords(max_region_tile_coords);

		for (int section_q = max_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (int section_r = max_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				RegionSection::Point section_coords{section_q, section_r};
				if (Section* s = section(section_coords)) {
					s->remove(light_source);
				}
			}
		}
	}

	double Region::illuminance(RegionTile::Point region_tile_coords) const
	{
		double result = _ambient_illuminance;
		if (Section const* s = containing_section(region_tile_coords)) {
			for (LightSource const& light_source : s->light_sources) {
				result += light_source.luminance(*this, region_tile_coords);
			}
		}
		return result;
	}

	double Region::temperature(RegionTile::Point region_tile_coords) const
	{
		return tile(region_tile_coords)->temperature_offset;
	}

	double Region::occlusion(RegionTile::Point start, RegionTile::Point end) const
	{
		auto line = start.line_to(end);
		double result = 1.0;
		for (std::size_t i = 1; i < line.size() - 1; ++i) {
			if (Being const* other = game().region().being(line[i])) {
				result *= other->transparency();
			}
			if (Object const* other = game().region().object(line[i])) {
				result *= other->transparency();
			}
		}
		return result;
	}

	void Region::update()
	{
		// Advance local time by one time unit per update.
		_time += 1.0;
		_time_of_day = get_time_of_day();
		_period_of_day = get_period_of_day();

		_ambient_illuminance = get_ambient_illuminance();

		std::vector<Id<Being>> beings_to_update;
		std::vector<Id<Object>> objects_to_update;
		for_each_loaded_section([&](Section& section) {
			for (Being& being : section.beings) {
				beings_to_update.push_back(being.id);
			}
			for (Object& object : section.objects) {
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

	Region::PeriodOfDay Region::get_period_of_day() const
	{
		if (_time_of_day <= _end_of_morning) {
			return PeriodOfDay::morning;
		} else if (_time_of_day <= _end_of_afternoon) {
			return PeriodOfDay::afternoon;
		} else if (_time_of_day <= _end_of_dusk) {
			return PeriodOfDay::dusk;
		} else if (_time_of_day <= _end_of_evening) {
			return PeriodOfDay::evening;
		} else if (_time_of_day <= _end_of_night) {
			return PeriodOfDay::night;
		} else {
			return PeriodOfDay::dawn;
		}
	}

	double Region::get_ambient_illuminance()
	{
		constexpr double night_light = 25.0;
		constexpr double dawn_and_dusk_light = 75.0;
		constexpr double noon_light = 115.0;

		switch (_period_of_day) {
			case PeriodOfDay::morning:
				[[fallthrough]];
			case PeriodOfDay::afternoon:
			{
				double const daylight_progress = sin(_time / _day_length * units::constants::tau);
				return dawn_and_dusk_light + daylight_progress * (noon_light - dawn_and_dusk_light);
			}
			case PeriodOfDay::dusk:
			{
				double const dusk_progress = (_time_of_day - _end_of_afternoon) / (_end_of_dusk - _end_of_afternoon);
				return (1.0 - dusk_progress) * dawn_and_dusk_light + dusk_progress * night_light;
			}
			case PeriodOfDay::evening:
				[[fallthrough]];
			case PeriodOfDay::night:
				return night_light;
			case PeriodOfDay::dawn:
			{
				double const dawn_progress = (_time_of_day - _end_of_night) / (_day_length - _end_of_night);
				return dawn_progress * dawn_and_dusk_light + (1.0 - dawn_progress) * night_light;
			}
			default:
				throw std::logic_error{"Invalid period of day."};
		}
	}

	void Region::for_each_loaded_section(function<void(Section&)> f)
	{
		for (int r = -_loaded_sections_q_radius; r <= _loaded_sections_q_radius; ++r) {
			for (int q = -_loaded_sections_r_radius; q <= _loaded_sections_r_radius; ++q) {
				RegionSection::Point section_coords{q, r};
				if (Section* s = section(section_coords)) {
					f(*s);
				}
			}
		}
	}

	Being* Region::being_helper(RegionTile::Point region_tile_coords) const
	{
		if (auto opt_id = being_id(region_tile_coords)) {
			return game().beings.get(*opt_id);
		} else {
			return nullptr;
		}
	}

	Object* Region::object_helper(RegionTile::Point region_tile_coords) const
	{
		if (auto opt_id = object_id(region_tile_coords)) {
			return game().objects.get(*opt_id);
		} else {
			return nullptr;
		}
	}

	Tile* Region::tile_helper(RegionTile::Point region_tile_coords) const
	{
		Section* section = containing_section_helper(region_tile_coords);
		if (section) {
			SectionTile::Point section_tile_coords = Section::section_tile_coords(region_tile_coords);
			return &section->tile(section_tile_coords);
		} else {
			return nullptr;
		}
	}
}
