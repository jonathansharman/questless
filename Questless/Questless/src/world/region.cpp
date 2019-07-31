//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "region.hpp"
#include "light_source.hpp"
#include "tile.hpp"

#include "agents/agent.hpp"
#include "agents/basic_ai.hpp"
#include "agents/lazy_ai.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "utility/random.hpp"
#include "utility/utility.hpp"

#include <limits.h>
#include <sstream>

using std::function;
using std::string;
using std::vector;
namespace fs = std::filesystem;

namespace ql {
	region::region(string region_name)
		: _name{std::move(region_name)}
		, _time{0}
		, _time_of_day{get_time_of_day()}
		, _period_of_day{get_period_of_day()}
		, _ambient_illuminance{get_ambient_illuminance()} //
	{
		auto const r_radius = 1_section_span;
		auto const q_radius = 1_section_span;

		for (section_span section_r = -r_radius; section_r <= r_radius; ++section_r) {
			for (section_span section_q = -q_radius; section_q <= q_radius; ++section_q) {
				region_section::point section_coords{section_q, section_r};

				// Create a section with random terrain.
				string data;
				for (span q = 0_span; q < section_diameter; ++q) {
					for (span r = 0_span; r < section_diameter; ++r) {
						// if (r == section_diameter - 1 || q == section_diameter - 1) {
						//	data += std::to_string(static_cast<int>(ql::subtype::edge)) + ' ' + std::to_string(0.0) + '
						//'; } else {
						data += std::to_string(uniform(0, static_cast<int>(terrain::terrain_count) - 1)) + ' ' +
							std::to_string(0.0) + ' ';
						//}

						// Every tile is snow (nice for testing lighting).
						// data += std::to_string(static_cast<int>(ql::subtype::snow)) + " 0 ";
					}
				}
				std::istringstream data_stream{data};
				_section_map.insert(std::make_pair(section_coords, section{section_coords, data_stream}));
				// Add beings randomly.
				for (span q = 0_span; q < section_diameter; ++q) {
					for (span r = 0_span; r < section_diameter; ++r) {
						if ((section_r != 0_section_span || section_q != 0_section_span) && uniform(0, 10) == 0) {
							auto entity_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});
							if (!uniform(0, 12)) {
								// Create and spawn campfire.
								ent campfire = reg.create();
								make_campfire(campfire, location{id, entity_coords});
								bool const success = try_spawn(campfire, entity_coords);
								assert(success);
							} else {
								// Create human.
								auto human_id = reg.create();
								make_human(human_id, location{id, entity_coords});
								// Give it a basic AI.
								reg.assign<basic_ai>(human_id, human_id);
								// Create quarterstaff.
								ent quarterstaff_id = reg.create();
								make_quarterstaff(quarterstaff_id); //! @todo Implement make_quarterstaff().
								// Give quarterstaff to human.
								reg.get<inventory>(human_id).add(quarterstaff_id);
								// Spawn human.
								bool const success = try_spawn(std::move(new_being), entity_coords);
								assert(success);
							}
						}
					}
				}
			}
		}
	}

	void region::save(char const* /*save_name*/) {
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

		fs::path const entities_filename{"entities.dat"};
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
#endif
	}

	std::optional<ent> region::entity_id_at(region_tile::point region_tile_coords) const {
		if (section const* section = containing_section(region_tile_coords)) {
			return section->entity_id_at(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	location region::get_spawn_location() {
		//! @todo More advanced spawning.

		// Spawn somewhere in the middle section.

		span q{uniform(-section_radius.value, section_radius.value)};
		span r{uniform(-section_radius.value, section_radius.value)};
		region_tile::point player_coords{q, r};

		// Erase the being currently there, if any.
		section* section = containing_section(player_coords);
		section->remove_at(player_coords);

		// Return the now guaranteed-empty location.
		return location{id, player_coords};
	}

	bool region::try_add(ent entity_id, region_tile::point region_tile_coords) {
		if (section* section = containing_section(region_tile_coords)) {
			auto& location = reg.get<ql::location>(entity_id);
			location.region_id = id;
			location.coords = region_tile_coords;

			return section->try_add(entity_id);
		} else {
			//! @todo What to do when adding outside current sections?
			return false;
		}
	}

	bool region::try_move(ent entity_id, region_tile::point region_tile_coords) {
		auto& location = reg.get<ql::location>(entity_id);
		if (location.region_id != id) {
			// The entity is not in this region to begin with.
			return false;
		}

		section& src_section = *containing_section(location.coords);
		section* dst_section = containing_section(region_tile_coords);
		if (dst_section != &src_section) {
			if (dst_section != nullptr) {
				if (dst_section->entity_id_at(region_tile_coords)) {
					// Collision with another entity. Prevent movement.
					return false;
				}

				src_section.remove(entity_id);
				location.coords = region_tile_coords;
				return dst_section->try_add(entity_id);
			} else {
				//! @todo Need to deal with null destination case.
				return false;
			}
		} else {
			if (src_section.entity_id_at(region_tile_coords)) {
				// Collision with another entity. Prevent movement.
				return false;
			}
			src_section.remove(entity_id);
			location.coords = region_tile_coords;
			return src_section.try_add(entity_id);
		}
		return true;
	}

	void region::remove(ent entity_id) {
		auto& location = reg.get<ql::location>(entity_id);
		if (section* section = containing_section(location.coords)) {
			remove_from_turn_queue(entity_id);
			section->remove(entity_id);
		}
	}

	void region::add(light_source const& light_source) {
		span range = light_source.range();
		span const min_q = light_source.coords().q - range;
		span const min_r = light_source.coords().r - range;
		region_tile::point const min_region_tile_coords{min_q, min_r};
		region_section::point const min_section_coords = section::region_section_coords(min_region_tile_coords);

		span const max_q = light_source.coords().q + range;
		span const max_r = light_source.coords().r + range;
		region_tile::point const max_region_tile_coords{max_q, max_r};
		region_section::point const max_section_coords = section::region_section_coords(max_region_tile_coords);

		for (section_span section_q = min_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (section_span section_r = min_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				region_section::point section_coords{section_q, section_r};
				if (section* s = section_at(section_coords)) { s->add(light_source); }
			}
		}
	}

	void region::remove(light_source const& light_source) {
		span const min_q = light_source.coords().q - light_source.range();
		span const min_r = light_source.coords().r - light_source.range();
		region_tile::point const min_region_tile_coords{min_q, min_r};
		region_section::point const min_section_coords = section::region_section_coords(min_region_tile_coords);

		span const max_q = light_source.coords().q + light_source.range();
		span const max_r = light_source.coords().r + light_source.range();
		region_tile::point const max_region_tile_coords{max_q, max_r};
		region_section::point const max_section_coords = section::region_section_coords(max_region_tile_coords);

		for (section_span section_q = max_section_coords.q; section_q <= max_section_coords.q; ++section_q) {
			for (section_span section_r = max_section_coords.r; section_r <= max_section_coords.r; ++section_r) {
				region_section::point section_coords{section_q, section_r};
				if (section* s = section_at(section_coords)) { s->remove(light_source); }
			}
		}
	}

	std::optional<ent> tile_at(region_tile::point region_tile_coords) {
		section* section = containing_section(region_tile_coords);
		if (section) {
			section_tile::point section_tile_coords = section::section_tile_coords(region_tile_coords);
			return section->tile_id_at(section_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	lum region::illuminance(region_tile::point region_tile_coords) const {
		lum result = _ambient_illuminance;
		if (section const* s = containing_section(region_tile_coords)) {
			for (light_source const& light_source : s->light_sources) {
				result += light_source.luminance(*this, region_tile_coords);
			}
		}
		return result;
	}

	ql::temperature region::temperature(region_tile::point region_tile_coords) const {
		if (auto tile_id = tile_at(region_tile_coords)) {
			return reg.get<tile>(*tile_id).temperature_offset;
		} else {
			return 0_temp;
		}
	}

	double region::occlusion(region_tile::point start, region_tile::point end) const {
		auto line = start.line_to(end);
		double result = 1.0;
		for (std::size_t i = 1; i < line.size() - 1; ++i) {
			//! @todo Handle transparency/occlusion in a more sensible way.
			if (entity_id_at(line[i])) { result *= 0.5; }
		}
		return result;
	}

	void region::update(tick elapsed) {
		_time += elapsed;
		_time_of_day = get_time_of_day();
		_period_of_day = get_period_of_day();

		_ambient_illuminance = get_ambient_illuminance();
	}

	void region::add_effect(effects::effect const& effect) {
		span range = effect.range();
		region_tile::point const origin = effect.origin();

		region_tile::point const min_tile_coords{origin.q - range, origin.r - range};
		region_tile::point const max_tile_coords{origin.q + range, origin.r + range};

		region_section::point const min_section_coords = section::region_section_coords(min_tile_coords);
		region_section::point const max_section_coords = section::region_section_coords(max_tile_coords);

		for (section_span r = min_section_coords.r; r <= max_section_coords.r; ++r) {
			for (section_span q = min_section_coords.q; q <= max_section_coords.q; ++q) {
				region_section::point section_coords{q, r};
				if (section* section = section_at(section_coords)) {
					for (being& being : section->beings) {
						being.agent().perceive(effect);
					}
				}
			}
		}
	}

	period_of_day region::get_period_of_day() const {
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

	lum region::get_ambient_illuminance() {
		constexpr auto night_light = 25.0_lum;
		constexpr auto dawn_and_dusk_light = 75.0_lum;
		constexpr auto noon_light = 115.0_lum;

		switch (_period_of_day) {
			case period_of_day::morning:
				[[fallthrough]];
			case period_of_day::afternoon: {
				double const daylight_progress = sin((_time / _day_length).value * vecx::constants::tau);
				return dawn_and_dusk_light + daylight_progress * (noon_light - dawn_and_dusk_light);
			}
			case period_of_day::dusk: {
				double const dusk_progress = ((_time_of_day - _end_of_afternoon) / (_end_of_dusk - _end_of_afternoon)).value;
				return (1.0 - dusk_progress) * dawn_and_dusk_light + dusk_progress * night_light;
			}
			case period_of_day::evening:
				[[fallthrough]];
			case period_of_day::night:
				return night_light;
			case period_of_day::dawn: {
				double const dawn_progress = ((_time_of_day - _end_of_night) / (_day_length - _end_of_night)).value;
				return dawn_progress * dawn_and_dusk_light + (1.0 - dawn_progress) * night_light;
			}
			default:
				throw std::logic_error{"Invalid period of day."};
		}
	}

	void region::for_each_loaded_section(std::function<void(section&)> const& f) {
		for (section_span r = -_loaded_sections_q_radius; r <= _loaded_sections_q_radius; ++r) {
			for (section_span q = -_loaded_sections_r_radius; q <= _loaded_sections_r_radius; ++q) {
				region_section::point section_coords{q, r};
				if (section* s = section_at(section_coords)) { f(*s); }
			}
		}
	}

	ent make_region(ent id, std::string name) {
		reg.assign<region>(reg.create(), std::move(name));
		return id;
	}
}
