//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "hex_space.hpp"

#include "quantities/distance.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "ui/view_space.hpp"

namespace ql {
	constexpr auto section_radius = 10_pace;
	constexpr auto section_diameter = 2 * section_radius + 1_pace;

	//! A point in the hex tile grid.
	using tile_hex_point = hex_point<pace>;

	//! A vector in the hex tile grid.
	using tile_hex_vector = hex_vector<pace>;

	//! A point in the region section grid.
	using section_hex_point = hex_point<section_span>;

	//! A vector in the region section grid.
	using section_hex_vector = hex_vector<section_span>;

	//! A location within the entire world.
	struct location {
		id region_id;
		tile_hex_point coords;
	};

	//! The hex layout used in Questless.
	constexpr hex_layout<pace, view::px> tile_layout{//
		hex_orientation::flat(),
		view::vector{view::px{30.0}, view::px{36.0} / gcem::sqrt(3.0f)},
		view::point{view::px{0.0}, view::px{0.0}}};
}

// Specialize std::hash.
namespace std {
	template <>
	struct hash<ql::tile_hex_point> {
		auto operator()(ql::tile_hex_point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::tile_hex_vector> {
		auto operator()(ql::tile_hex_vector const& v) const {
			return hash_value(v);
		}
	};

	template <>
	struct hash<ql::section_hex_point> {
		auto operator()(ql::section_hex_point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::section_hex_vector> {
		auto operator()(ql::section_hex_vector const& v) const {
			return hash_value(v);
		}
	};
}
