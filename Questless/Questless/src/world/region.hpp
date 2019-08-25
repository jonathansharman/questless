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
		//! @param id The ID of the new region.
		//! @param name The name of the new region.
		region(ql::id id, std::string region_name);

		//! The region's name.
		std::string name() const {
			return _name;
		}

		//! The ID of the entity at @p tile_coords or nullopt if none.
		std::optional<ql::id> entity_id_at(tile_hex::point tile_coords) const;

		//! Finds a suitable spawn location.
		location get_spawn_location();

		//! Adds @p entity_id to the region, setting its coordinates to @p tile_coords.
		[[nodiscard]] auto try_add(ql::id entity_id, tile_hex::point tile_coords) -> bool;

		//! Moves @p entity_id to the tile at @p tile_coords.
		//! @return Whether @p being was successfully moved.
		[[nodiscard]] auto try_move(ql::id entity_id, tile_hex::point tile_coords) -> bool;

		//! Removes @p entity_id from the region, if present.
		void remove(ql::id entity_id);

		//! The tile at @p tile_coords or nullopt if none.
		auto tile_id_at(tile_hex::point tile_coords) const -> std::optional<ql::id>;

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

		//! The illuminance of the tile at @p tile_coords.
		lum illuminance(tile_hex::point tile_coords) const;

		//! The temperature of the tile at @p tile_coords.
		ql::temperature temperature(tile_hex::point tile_coords) const;

		//! The proportion of light/vision occluded between @p start and @p end, as a number in [0, 1].
		double occlusion(tile_hex::point start, tile_hex::point end) const;

		//! Advances this region by @elapsed time.
		void update(tick elapsed);

		//! Adds an effect to this region, notifying beings within range of its occurrence.
		//! @param effect The effect to add.
		void add_effect(effects::effect const& effect);

	private:
		/////////////////
		// Member Data //
		/////////////////

		std::string _name;
		std::map<section_hex::point, section> _section_map;
		section_hex::point center_section_coords{0_section_span, 0_section_span};

		tick _time;
		tick _time_of_day;
		ql::period_of_day _period_of_day;

		lum _ambient_illuminance;

		//////////////////////
		// Member Functions //
		//////////////////////

		//! The section that contains @p tile_coords or nullptr if none.
		auto containing_section(tile_hex::point tile_coords) -> section*;

		//! The section that contains @p tile_coords or nullptr if none.
		auto containing_section(tile_hex::point tile_coords) const -> section const*;

		auto get_time_of_day() const -> tick;
		auto get_period_of_day() const -> ql::period_of_day;

		lum get_ambient_illuminance();

		//! Performs some operation on each section in the loaded rhomboid of sections.
		//! @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(section&)> const& f);
	};

	auto make_region(id region_id, std::string name) -> id;
}
