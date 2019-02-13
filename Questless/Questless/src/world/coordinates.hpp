//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "world.hpp"

#include "quantities/distance.hpp"
#include "quantities/quantities.hpp"

#include "vecx/hex_space.hpp"

namespace ql {
	struct region;

	//! The space of tiles in a section.
	using section_tile = vecx::hex_space<struct section_tile_tag, span>;

	//! The space of tiles in a region.
	using region_tile = vecx::hex_space<struct region_tile_tag, span>;

	//! The space of sections in a region.
	using region_section = vecx::hex_space<struct region_section_tag, section_span>;

	//! Location within the entire world.
	struct location {
		region* region;
		region_tile::point coords;
	};

	//! The hex layout used in Questless.
	constexpr auto hex_layout = vecx::hex_layout{vecx::hex_orientation::flat(),
		world::vector{world::length{30.0}, world::length{36.0} / gcem::sqrt(3.0f)},
		world::point{world::length{0.0}, world::length{0.0}}};

	// Conversions between world and region tile coordinates.

	//! Converts @p p to a world space point.
	constexpr auto to_world(region_tile::point p) {
		return world::point{
			((hex_layout.orientation.f0 * p.q.value + hex_layout.orientation.f1 * p.r.value) * x(hex_layout.size) +
				x(hex_layout.origin)),
			((hex_layout.orientation.f2 * p.q.value + hex_layout.orientation.f3 * p.r.value) * y(hex_layout.size) +
				y(hex_layout.origin))};
	}
	//! Converts @p v to a world space vector.
	constexpr auto to_world(region_tile::vector v) {
		return world::point{
			((hex_layout.orientation.f0 * v.q.value + hex_layout.orientation.f1 * v.r.value) * x(hex_layout.size)),
			((hex_layout.orientation.f2 * v.q.value + hex_layout.orientation.f3 * v.r.value) * y(hex_layout.size))};
	}

	//! Converts @p p to a region tile space point.
	constexpr auto to_region_tile(world::point p) {
		auto const r0 = hex_layout.orientation.b0 * (x(p) - x(hex_layout.origin)) / x(hex_layout.size);
		auto const r1 = hex_layout.orientation.b1 * (y(p) - y(hex_layout.origin)) / y(hex_layout.size);
		auto const r2 = hex_layout.orientation.b2 * (x(p) - x(hex_layout.origin)) / x(hex_layout.size);
		auto const r3 = hex_layout.orientation.b3 * (y(p) - y(hex_layout.origin)) / y(hex_layout.size);
		return region_tile::point{(r0 + r1).value, (r2 + r3).value};
	}
	//! Converts @p v to a region tile space vector.
	constexpr auto to_region_tile(world::vector v) {
		auto const r0 = hex_layout.orientation.b0 * x(v) / x(hex_layout.size);
		auto const r1 = hex_layout.orientation.b1 * y(v) / y(hex_layout.size);
		auto const r2 = hex_layout.orientation.b2 * x(v) / x(hex_layout.size);
		auto const r3 = hex_layout.orientation.b3 * y(v) / y(hex_layout.size);
		return region_tile::vector{(r0 + r1).value, (r2 + r3).value};
	}
}

// Specialize std::hash.
namespace std {
	template <>
	struct hash<ql::section_tile::point> {
		size_t operator()(ql::section_tile::point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::section_tile::vector> {
		size_t operator()(ql::section_tile::vector const& v) const {
			return hash_value(v);
		}
	};

	template <>
	struct hash<ql::region_tile::point> {
		size_t operator()(ql::region_tile::point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::region_tile::vector> {
		size_t operator()(ql::region_tile::vector const& v) const {
			return hash_value(v);
		}
	};

	template <>
	struct hash<ql::region_section::point> {
		size_t operator()(ql::region_section::point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::region_section::vector> {
		size_t operator()(ql::region_section::vector const& v) const {
			return hash_value(v);
		}
	};
}
