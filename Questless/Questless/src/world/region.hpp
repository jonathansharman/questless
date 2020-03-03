//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

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
		region(reg& reg, ql::id id, std::string region_name);

		//! The region's name.
		auto name() const -> std::string {
			return _name;
		}

		//! The ID of the entity at @p tile_coords or nullopt if none.
		auto entity_id_at(tile_hex::point tile_coords) const -> std::optional<ql::id>;

		//! Finds a suitable spawn location.
		auto get_spawn_location() -> location;

		//! Adds @p entity_id to the region, setting its coordinates to @p tile_coords.
		[[nodiscard]] auto try_add(ql::id entity_id, tile_hex::point tile_coords) -> bool;

		//! Moves @p entity_id to the tile at @p tile_coords.
		//! @return Whether @p being was successfully moved.
		[[nodiscard]] auto try_move(ql::id entity_id, tile_hex::point tile_coords) -> bool;

		//! Removes @p entity_id from the region, if present.
		auto remove(ql::id entity_id) -> void;

		//! The tile at @p tile_coords or nullopt if none.
		auto tile_id_at(tile_hex::point tile_coords) const -> std::optional<ql::id>;

		//! The total in-game time in this region.
		auto time() const -> tick {
			return _time;
		}

		//! The time of day in this region.
		auto time_of_day() const -> tick {
			return _time_of_day;
		}

		//! The period of day in this region.
		auto period_of_day() const -> period_of_day {
			return _period_of_day;
		}

		//! The illuminance of the tile at @p tile_coords.
		auto illuminance(tile_hex::point tile_coords) const -> lum;

		//! The temperature of the tile at @p tile_coords.
		auto temperature(tile_hex::point tile_coords) const -> ql::temperature;

		//! The proportion of light/vision occluded between @p start and @p end, as a number in [0, 1].
		auto occlusion(tile_hex::point start, tile_hex::point end) const -> double;

		//! Advances this region by @elapsed time.
		auto update(tick elapsed) -> void;

		//! Adds an effect to this region, notifying beings within range of its occurrence.
		//! @param effect The effect to add.
		auto add_effect(effects::effect const& effect) -> void;

	private:
		reg_ptr _reg;

		std::string _name;
		std::map<section_hex::point, section> _section_map;
		section_hex::point center_section_coords{0_section_span, 0_section_span};

		tick _time;
		tick _time_of_day;
		ql::period_of_day _period_of_day;

		lum _ambient_illuminance;

		//! The section that contains @p tile_coords or nullptr if none.
		auto containing_section(tile_hex::point tile_coords) -> section*;

		//! The section that contains @p tile_coords or nullptr if none.
		auto containing_section(tile_hex::point tile_coords) const -> section const*;

		auto get_time_of_day() const -> tick;
		auto get_period_of_day() const -> ql::period_of_day;

		auto get_ambient_illuminance() -> lum;

		//! Performs some operation on each section in the loaded rhomboid of sections.
		//! @param f The operation to perform on each section.
		auto for_each_loaded_section(std::function<void(section&)> const& f) -> void;
	};

	auto make_region(reg& reg, id region_id, std::string name) -> id;
}
