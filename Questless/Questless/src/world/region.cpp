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
#include "entities/beings/human.hpp"
#include "entities/objects/campfire.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "utility/random.hpp"
#include "utility/utility.hpp"

#include <climits>
#include <optional>

namespace ql {
	namespace {
		// Time Constants

		static constexpr tick day_length = 12'000_tick;
		static constexpr tick end_of_morning = 25 * day_length / 100;
		static constexpr tick end_of_afternoon = 50 * day_length / 100;
		static constexpr tick end_of_dusk = 55 * day_length / 100;
		static constexpr tick end_of_evening = 67 * day_length / 100;
		static constexpr tick end_of_night = 95 * day_length / 100;
	}

	region::region(std::string region_name)
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

				// Generate a new random section.
				_section_map.insert(std::make_pair(section_coords, section{id, section_coords}));

				// Add beings randomly.
				for (span q = 0_span; q < section_diameter; ++q) {
					for (span r = 0_span; r < section_diameter; ++r) {
						if ((section_r != 0_section_span || section_q != 0_section_span) && uniform(0, 10) == 0) {
							auto entity_coords = section::region_tile_coords(section_coords, section_tile::point{q, r});
							if (!uniform(0, 12)) {
								// Create and spawn campfire.
								auto campfire_id = reg.create();
								make_campfire(campfire_id, location{id, entity_coords});
								bool const success = try_add(campfire_id, entity_coords);
								assert(success);
							} else {
								// Create human.
								auto human_id = reg.create();
								make_human(human_id, location{id, entity_coords}, {basic_ai{human_id, human_id}});
								// Create quarterstaff.
								auto quarterstaff_id = reg.create();
								make_quarterstaff(quarterstaff_id);
								// Give quarterstaff to human.
								reg.get<inventory>(human_id).add(quarterstaff_id);
								// Spawn human.
								bool const success = try_add(std::move(human_id), entity_coords);
								assert(success);
							}
						}
					}
				}
			}
		}
	}

	auto region::entity_id_at(region_tile::point region_tile_coords) const -> std::optional<ql::id> {
		if (auto section = containing_section(region_tile_coords)) {
			section->entity_id_at(region_tile_coords);
		} else {
			return std::nullopt;
		}
	}

	auto region::get_spawn_location() -> location {
		//! @todo More advanced spawning.

		// Spawn somewhere in the middle section.

		span q{uniform(-section_radius, section_radius)};
		span r{uniform(-section_radius, section_radius)};
		region_tile::point player_coords{q, r};

		// Destroy and remove the entity currently there, if any.
		if (auto const o_entity_id = entity_id_at(player_coords)) {
			auto const entity_id = *o_entity_id;
			reg.destroy(entity_id);
			remove(entity_id);
		}

		// Return the now guaranteed-empty location.
		return location{id, player_coords};
	}

	auto region::try_add(ql::id entity_id, region_tile::point region_tile_coords) -> bool {
		if (auto section = containing_section(region_tile_coords)) {
			auto& location = reg.get<ql::location>(entity_id);
			location.region_id = id;
			location.coords = region_tile_coords;

			return section->try_add(entity_id);
		} else {
			//! @todo What to do when adding outside current sections?
			return false;
		}
	}

	auto region::try_move(ql::id entity_id, region_tile::point region_tile_coords) -> bool {
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

	auto region::remove(ql::id entity_id) -> void {
		auto& location = reg.get<ql::location>(entity_id);
		if (auto section = containing_section(location.coords)) { section->remove(entity_id); }
	}

	auto region::tile_id_at(region_tile::point region_tile_coords) const -> std::optional<ql::id> {
		if (auto section = containing_section(region_tile_coords)) {
			return section->tile_id_at(section::section_tile_coords(region_tile_coords));
		} else {
			return std::nullopt;
		}
	}

	auto region::containing_section(region_tile::point region_tile_coords) -> section* {
		auto it = _section_map.find(region_section_coords(region_tile_coords));
		return it == _section_map.end() ? nullptr : &it->second;
	}

	auto region::containing_section(region_tile::point region_tile_coords) const -> section const* {
		return static_cast<region const&>(*this).containing_section(region_tile_coords);
	}

	auto region::illuminance(region_tile::point region_tile_coords) const -> lum {
		if (auto tile_id = tile_id_at(region_tile_coords)) {
			return _ambient_illuminance + reg.get<lum>(*tile_id);
		} else {
			return _ambient_illuminance;
		}
	}

	auto region::temperature(region_tile::point region_tile_coords) const -> ql::temperature {
		if (auto tile_id = tile_id_at(region_tile_coords)) {
			return reg.get<ql::temperature>(*tile_id);
		} else {
			return 0_temp;
		}
	}

	auto region::occlusion(region_tile::point start, region_tile::point end) const -> double {
		auto line = start.line_to(end);
		double result = 1.0;
		for (std::size_t i = 1; i < line.size() - 1; ++i) {
			//! @todo Handle transparency/occlusion in a more sensible way.
			if (entity_id_at(line[i])) { result *= 0.5; }
		}
		return result;
	}

	auto region::update(tick elapsed) -> void {
		_time += elapsed;
		_time_of_day = get_time_of_day();
		_period_of_day = get_period_of_day();

		_ambient_illuminance = get_ambient_illuminance();
	}

	auto region::add_effect(effects::effect const& effect) -> void {
		// Each agent within range in loaded sections perceives the effect.
		for_each_loaded_section([&](section& section) {
			for (auto& [coords, id] : section.entity_id_map) {
				if ((coords - effect.origin()).length() <= effect.range()) {
					if (agent* agent = reg.try_get<ql::agent>(id)) { agent->perceive(effect); }
				}
			}
		});
	}

	auto region::get_time_of_day() const -> tick {
		return _time % day_length;
	}

	auto region::get_period_of_day() const -> ql::period_of_day {
		if (_time_of_day <= end_of_morning) {
			return period_of_day::morning;
		} else if (_time_of_day <= end_of_afternoon) {
			return period_of_day::afternoon;
		} else if (_time_of_day <= end_of_dusk) {
			return period_of_day::dusk;
		} else if (_time_of_day <= end_of_evening) {
			return period_of_day::evening;
		} else if (_time_of_day <= end_of_night) {
			return period_of_day::night;
		} else {
			return period_of_day::dawn;
		}
	}

	auto region::get_ambient_illuminance() -> lum {
		constexpr auto night_light = 25_lum;
		constexpr auto dawn_and_dusk_light = 75_lum;
		constexpr auto noon_light = 115_lum;

		switch (_period_of_day) {
			case period_of_day::morning:
				[[fallthrough]];
			case period_of_day::afternoon: {
				double const daylight_progress = sin(ratio<double>(_time, day_length) * vecx::constants::tau);
				return dawn_and_dusk_light + scale(noon_light - dawn_and_dusk_light, daylight_progress);
			}
			case period_of_day::dusk: {
				double const dusk_progress = ratio<double>(_time_of_day - end_of_afternoon, end_of_dusk - end_of_afternoon);
				return scale(dawn_and_dusk_light, 1.0 - dusk_progress) + scale(night_light, dusk_progress);
			}
			case period_of_day::evening:
				[[fallthrough]];
			case period_of_day::night:
				return night_light;
			case period_of_day::dawn: {
				double const dawn_progress = ratio<double>(_time_of_day - end_of_night, day_length - end_of_night);
				return scale(dawn_and_dusk_light, dawn_progress) + scale(night_light, 1.0 - dawn_progress);
			}
			default:
				throw std::logic_error{"Invalid period of day."};
		}
	}

	auto region::for_each_loaded_section(std::function<void(section&)> const& f) -> void {
		static constexpr auto loaded_sections_q_radius = 1_section_span;
		static constexpr auto loaded_sections_r_radius = 1_section_span;

		for (section_span r = -loaded_sections_q_radius; r <= loaded_sections_q_radius; ++r) {
			for (section_span q = -loaded_sections_r_radius; q <= loaded_sections_r_radius; ++q) {
				region_section::point const section_coords{q, r};
				if (auto it = _section_map.find(section_coords); it != _section_map.end()) { f(it->second); }
			}
		}
	}

	auto make_region(id region_id, std::string name) -> id {
		reg.assign<region>(reg.create(), std::move(name));
		return region_id;
	}
}
