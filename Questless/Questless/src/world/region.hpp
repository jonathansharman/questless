//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "section.hpp"

#include "quantities/misc.hpp"

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace ql {
	namespace effects {
		struct effect;
	}

	enum class period_of_day { morning, afternoon, dusk, evening, night, dawn };

	//! A large set of connected sections of hexagonal tiles.
	struct region {
		id id;

		//! Pseudo-randomly generates a new region.
		//! @param name The name of the new region.
		region(std::string region_name);

		//! The region's name.
		std::string name() const {
			return _name;
		}

		//! Saves the region to disk.
		//! @param save_name The name of the region's save file.
		void save(char const* save_name);

		//! The ID of the entity at @p region_tile_coords or nullopt if none.
		std::optional<ql::id> entity_id_at(region_tile::point region_tile_coords) const;

		//! Finds a suitable spawn location.
		location get_spawn_location();

		//! Adds @p entity_id to the region, setting its coordinates to @p region_tile_coords.
		[[nodiscard]] bool try_add(ql::id entity_id, region_tile::point region_tile_coords);

		//! Moves @p entity_id to the tile at @p region_tile_coords.
		//! @return Whether @p being was successfully moved.
		[[nodiscard]] bool try_move(ql::id entity_id, region_tile::point region_tile_coords);

		//! Removes @p entity_id from the region, if present.
		void remove(ql::id entity_id);

		//! Adds @p light_source to the region's light sources.
		void add(light_source const& light_source);

		//! Removes @p light_source from the region's light sources, if present.
		void remove(light_source const& light_source);

		//! The tile at @p region_tile_coords or nullopt if none.
		std::optional<ql::id> tile_at(region_tile::point region_tile_coords) const;

		//! The section at @p region_section_coords.
		section const* section_at(region_section::point region_section_coords) const {
			auto it = _section_map.find(region_section_coords);
			return it == _section_map.end() ? nullptr : const_cast<section*>(&it->second);
		}

		//! The section that contains @p region_tile_coords or nullptr if none.
		section const* containing_section(region_tile::point region_tile_coords) const {
			return containing_section_helper(region_tile_coords);
		}

		//! The total in-game time in this region.
		tick time() const {
			return _time;
		}

		//! The time of day in this region.
		tick time_of_day() const {
			return _time_of_day;
		}

		//! The period of day in this region.
		period_of_day period_of_day() const {
			return _period_of_day;
		}

		//! The illuminance of the tile at @p region_tile_coords.
		lum illuminance(region_tile::point region_tile_coords) const;

		//! The temperature of the tile at @p region_tile_coords.
		ql::temperature temperature(region_tile::point region_tile_coords) const;

		//! The proportion of light/vision occluded between @p start and @p end, as a number in [0, 1].
		double occlusion(region_tile::point start, region_tile::point end) const;

		//! Advances this region by @elapsed time.
		void update(tick elapsed);

		//! Adds an effect to this region, notifying beings within range of its occurrence.
		//! @param effect The effect to add.
		void add_effect(effects::effect const& effect);

	private:
		///////////////
		// Constants //
		///////////////

		// These define the maximum size of the sections rhomboid that should be loaded at any given time.

		static constexpr auto _loaded_sections_q_radius = 1_section_span;
		static constexpr auto _loaded_sections_r_radius = 1_section_span;

		// Time Constants

		static constexpr tick _day_length = 12'000_tick;
		static constexpr double _twilight_pct = 0.05;
		static constexpr tick _end_of_morning = _day_length / 4;
		static constexpr tick _end_of_afternoon = _day_length / 2;
		static constexpr tick _end_of_dusk = cancel::quantity_cast<tick>((0.5 + _twilight_pct) * _day_length);
		static constexpr tick _end_of_evening = 2 * _day_length / 3;
		static constexpr tick _end_of_night = cancel::quantity_cast<tick>((1.0 - _twilight_pct) * _day_length);

		/////////////////
		// Member Data //
		/////////////////

		std::string _name;
		std::map<region_section::point, section> _section_map;
		region_section::point center_section_coords{0_span, 0_span};

		tick _time;
		tick _time_of_day;
		ql::period_of_day _period_of_day;

		lum _ambient_illuminance;

		//////////////////////
		// Member Functions //
		//////////////////////

		tick get_time_of_day() const {
			return _time % _day_length;
		}
		ql::period_of_day get_period_of_day() const;

		lum get_ambient_illuminance();

		//! Performs some operation on each section in the loaded rhomboid of sections.
		//! @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(section&)> const& f);

		// Helper methods to avoid duplicating code for const and non-const versions of methods.

		section* containing_section_helper(region_tile::point region_tile_coords) const {
			auto it = _section_map.find(region_section_coords(region_tile_coords));
			return it == _section_map.end() ? nullptr : const_cast<section*>(&it->second);
		}
	};

	id make_region(id id, std::string name);
}
