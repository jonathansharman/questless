//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <filesystem>
namespace fs = std::experimental::filesystem; //! @todo Replace this with std::filesystem when available.
#include <limits.h>
#include <sstream>
#include <fstream>

#include "agents/agent.hpp"
#include "agents/lazy_ai.hpp"
#include "agents/basic_ai.hpp"
#include "entities/all_entities.hpp"
#include "game.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "units/constants.hpp"
#include "utility/random.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

using std::string;
using std::vector;
using std::function;

using namespace sdl;

namespace ql
{
	bool turn_order_function(being const& first, being const& second)
	{
		// Sort beings in the turn queue by lower busy-time first, then lower entity ID.
		double f_b = first.busy_time;
		double s_b = second.busy_time;
		return f_b < s_b || (f_b == s_b && first.id < second.id);
	}

	region::region(string region_name)
		: _name{std::move(region_name)}
		, _time{0.0}
		, _turn_queue{turn_order_function}
		, _ambient_illuminance{get_ambient_illuminance()}
	{
		int const r_radius = 1;
		int const q_radius = 1;

		for (int section_r = -r_radius; section_r <= r_radius; ++section_r) {
			for (int section_q = -q_radius; section_q <= q_radius; ++section_q) {
				region_section::point section_coords{section_q, section_r};

				// Create a section with random terrain.
				string data;
				for (int q = 0; q < section::diameter; ++q) {
					for (int r = 0; r < section::diameter; ++r) {
						//if (r == section::diameter - 1 || q == section::diameter - 1) {
						//	data += std::to_string(static_cast<int>(ql::subtype::edge)) + ' ' + std::to_string(0.0) + ' ';
						//} else {
							data += std::to_string(uniform(0, static_cast<int>(tile_subtype::TILE_CLASS_COUNT) - 1)) + ' ' + std::to_string(0.0) + ' ';
						//}

						// Every tile is snow (nice for testing lighting).
						//data += std::to_string(static_cast<int>(ql::subtype::snow)) + " 0 ";
					}
				}
				std::istringstream data_stream{data};
				_section_map.insert(std::make_pair(section_coords, section{section_coords, data_stream}));
				// Add beings randomly.
				for (int q = 0; q < section::diameter; ++q) {
					for (int r = 0; r < section::diameter; ++r) {
						if ((section_r != 0 || section_q != 0) && uniform(0, 10) == 0) {
							auto entity_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});
							if (!uniform(0, 12)) {
								spawn(umake<campfire>(*this, entity_coords), entity_coords);
							} else {
								auto new_being = umake<goblin>(agent::make<basic_ai>);
								new_being->inventory.add(the_game().items.add(umake<quarterstaff>()).id);
								spawn(std::move(new_being), entity_coords);
							}
						}
					}
				}
			}
		}
	}

	region::region(char const* save_name, string region_name)
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
			region_section::point section_coords{section_q, section_r};

			std::ifstream data_stream{(region_path / it->path()).string()};
			if (data_stream.fail()) {
				throw std::logic_error("Could not open section file.");
			}
			_section_map.insert(std::make_pair(section_coords, section{section_coords, data_stream}));
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
			switch (static_cast<entity_class>(entity_id)) {
				case entity_class::goblin_class:
				{
					being& goblin = the_game().beings.add(umake<ql::goblin>(sin));
					add(goblin, goblin.coords);
					break;
				}
				case entity_class::troll_class:
					break;
				default:
					throw std::logic_error("Unknown entity type in entities file.");
			}
		}
	}

	void region::save(char const* /*save_name*/)
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

	being* region::next_ready_being()
	{
		if (_turn_queue.empty()) {
			return nullptr;
		} else {
			being& next = *_turn_queue.begin();
			return &next;
		}
	}

	std::optional<id<being>> region::being_id_at(region_tile::point region_tile_coords) const
	{
		if (section const* section = containing_section(region_tile_coords)) {
			return section->being_id(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	std::optional<id<object>> region::object_id_at(region_tile::point region_tile_coords) const
	{
		if (section const* section = containing_section(region_tile_coords)) {
			return section->object_id(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	void region::spawn_player(uptr<being> player_being)
	{
		//! @todo More advanced player spawning.

		// Put the player's character somewhere in the middle section.

		int q = uniform(-section::radius, section::radius);
		int r = uniform(-section::radius, section::radius);
		region_tile::point player_coords{q, r};

		// Erase the being currently there, if any.
		section* section = containing_section(player_coords);
		section->remove_being(player_coords);

		spawn(std::move(player_being), player_coords);
	}

	void region::add(being& being, region_tile::point region_tile_coords)
	{
		if (section* section = containing_section(region_tile_coords)) {
			being.region = this;
			being.section = section;
			being.coords = region_tile_coords;

			add_to_turn_queue(being);
			section->add(being);
		} else {
			//! @todo What to do when adding outside current sections?
		}
	}

	void region::add(ql::object& object, region_tile::point region_tile_coords)
	{
		if (section* section = containing_section(region_tile_coords)) {
			object.region = this;
			object.section = section;
			object.coords = region_tile_coords;

			section->add(object);
		} else {
			//! @todo What to do when adding outside current sections?
		}
	}

	void region::spawn(uptr<being> being, region_tile::point region_tile_coords)
	{
		add(the_game().beings.add(std::move(being)), region_tile_coords);
	}

	void region::spawn(uptr<ql::object> object, region_tile::point region_tile_coords)
	{
		add(the_game().objects.add(std::move(object)), region_tile_coords);
	}

	bool region::move(being& being, region_tile::point region_tile_coords)
	{
		section& src_section = *being.section;
		section* dst_section = containing_section(region_tile_coords);
		if (dst_section != &src_section) {
			if (dst_section != nullptr) {
				if (dst_section->being_id(region_tile_coords)) {
					// Collision with another being. Prevent movement.
					return false;
				}
				if (auto object_id = dst_section->object_id(region_tile_coords)) {
					if (the_game().objects.cref(*object_id).blocks_movement()) {
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
				if (the_game().objects.cref(*object_id).blocks_movement()) {
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

	bool region::move(ql::object& object, region_tile::point region_tile_coords)
	{
		section& src_section = *object.section;
		section* dst_section = containing_section(region_tile_coords);
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
	
	void region::remove(being& being)
	{
		section& section = *being.section;

		being.region = nullptr;
		being.section = nullptr;

		remove_from_turn_queue(being);
		section.remove(being);
	}

	void region::remove(ql::object& object)
	{
		section& section = *object.section;

		object.region = nullptr;
		object.section = nullptr;

		section.remove(object);
	}

	void region::add(light_source const& light_source)
	{
		int range = light_source.range();
		int const min_q = light_source.coords().q - range;
		int const min_r = light_source.coords().r - range;
		region_tile::point const min_region_tile_coords{min_q, min_r};
		region_section::point const min_section_coords = section::region_section_coords(min_region_tile_coords);

		int const max_q = light_source.coords().q + range;
		int const max_r = light_source.coords().r + range;
		region_tile::point const max_region_tile_coords{max_q, max_r};
		region_section::point const max_section_coords = section::region_section_coords(max_region_tile_coords);

		for (int section_q = min_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (int section_r = min_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				region_section::point section_coords{section_q, section_r};
				if (section* s = section_at(section_coords)) {
					s->add(light_source);
				}
			}
		}
	}

	void region::remove(light_source const& light_source)
	{
		int const min_q = light_source.coords().q - light_source.range();
		int const min_r = light_source.coords().r - light_source.range();
		region_tile::point const min_region_tile_coords{min_q, min_r};
		region_section::point const min_section_coords = section::region_section_coords(min_region_tile_coords);

		int const max_q = light_source.coords().q + light_source.range();
		int const max_r = light_source.coords().r + light_source.range();
		region_tile::point const max_region_tile_coords{max_q, max_r};
		region_section::point const max_section_coords = section::region_section_coords(max_region_tile_coords);

		for (int section_q = max_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (int section_r = max_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				region_section::point section_coords{section_q, section_r};
				if (section* s = section_at(section_coords)) {
					s->remove(light_source);
				}
			}
		}
	}

	double region::illuminance(region_tile::point region_tile_coords) const
	{
		double result = _ambient_illuminance;
		if (section const* s = containing_section(region_tile_coords)) {
			for (light_source const& light_source : s->light_sources) {
				result += light_source.luminance(*this, region_tile_coords);
			}
		}
		return result;
	}

	double region::temperature(region_tile::point region_tile_coords) const
	{
		return tile_at(region_tile_coords)->temperature_offset;
	}

	double region::occlusion(region_tile::point start, region_tile::point end) const
	{
		auto line = start.line_to(end);
		double result = 1.0;
		for (std::size_t i = 1; i < line.size() - 1; ++i) {
			if (being const* other = the_game().region().being_at(line[i])) {
				result *= other->transparency();
			}
			if (object const* other = the_game().region().object_at(line[i])) {
				result *= other->transparency();
			}
		}
		return result;
	}

	void region::update()
	{
		// Advance local time by one time unit per update.
		_time += 1.0;
		_time_of_day = get_time_of_day();
		_period_of_day = get_period_of_day();

		_ambient_illuminance = get_ambient_illuminance();

		std::vector<id<being>> beings_to_update;
		std::vector<id<object>> objects_to_update;
		for_each_loaded_section([&](section& section) {
			for (being& being : section.beings) {
				beings_to_update.push_back(being.id);
			}
			for (object& object : section.objects) {
				objects_to_update.push_back(object.id);
			}
		});
		for (id<being> being_id : beings_to_update) {
			if (being* being = the_game().beings.ptr(being_id)) {
				being->update();
			}
		}
		for (id<object> object_id : objects_to_update) {
			if (object* object = the_game().objects.ptr(object_id)) {
				object->update();
			}
		}
	}

	period_of_day region::get_period_of_day() const
	{
		if (_time_of_day <= _end_of_morning) {
			return period_of_day::morning;
		} else if (_time_of_day <= _end_of_afternoon) {
			return period_of_day::afternoon;
		} else if (_time_of_day <= _end_of_dusk) {
			return period_of_day::dusk;
		} else if (_time_of_day <= _end_of_evening) {
			return period_of_day::evening;
		} else if (_time_of_day <= _end_of_night) {
			return period_of_day::night;
		} else {
			return period_of_day::dawn;
		}
	}

	double region::get_ambient_illuminance()
	{
		constexpr double night_light = 25.0;
		constexpr double dawn_and_dusk_light = 75.0;
		constexpr double noon_light = 115.0;

		switch (_period_of_day) {
			case period_of_day::morning:
				[[fallthrough]];
			case period_of_day::afternoon:
			{
				double const daylight_progress = sin(_time / _day_length * units::constants::tau);
				return dawn_and_dusk_light + daylight_progress * (noon_light - dawn_and_dusk_light);
			}
			case period_of_day::dusk:
			{
				double const dusk_progress = (_time_of_day - _end_of_afternoon) / (_end_of_dusk - _end_of_afternoon);
				return (1.0 - dusk_progress) * dawn_and_dusk_light + dusk_progress * night_light;
			}
			case period_of_day::evening:
				[[fallthrough]];
			case period_of_day::night:
				return night_light;
			case period_of_day::dawn:
			{
				double const dawn_progress = (_time_of_day - _end_of_night) / (_day_length - _end_of_night);
				return dawn_progress * dawn_and_dusk_light + (1.0 - dawn_progress) * night_light;
			}
			default:
				throw std::logic_error{"Invalid period of day."};
		}
	}

	void region::for_each_loaded_section(function<void(section&)> f)
	{
		for (int r = -_loaded_sections_q_radius; r <= _loaded_sections_q_radius; ++r) {
			for (int q = -_loaded_sections_r_radius; q <= _loaded_sections_r_radius; ++q) {
				region_section::point section_coords{q, r};
				if (section* s = section_at(section_coords)) {
					f(*s);
				}
			}
		}
	}

	being* region::being_helper(region_tile::point region_tile_coords) const
	{
		if (auto opt_id = being_id_at(region_tile_coords)) {
			return the_game().beings.ptr(*opt_id);
		} else {
			return nullptr;
		}
	}

	object* region::object_helper(region_tile::point region_tile_coords) const
	{
		if (auto opt_id = object_id_at(region_tile_coords)) {
			return the_game().objects.ptr(*opt_id);
		} else {
			return nullptr;
		}
	}

	tile* region::tile_helper(region_tile::point region_tile_coords) const
	{
		section* section = containing_section_helper(region_tile_coords);
		if (section) {
			section_tile::point section_tile_coords = section::section_tile_coords(region_tile_coords);
			return &section->tile_at(section_tile_coords);
		} else {
			return nullptr;
		}
	}
}
