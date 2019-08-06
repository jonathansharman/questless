//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "hex_space.hpp"

#include "quantities/distance.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "ui/view_space.hpp"

namespace ql {
	constexpr auto section_radius = 10_span;
	constexpr auto section_diameter = 2 * section_radius + 1_span;

	//! The space of tiles in a region.
	using tile_hex = hex_space<struct tile_hex_tag, span>;

	//! The space of sections in a region.
	using section_hex = hex_space<struct section_hex_tag, section_span>;

	//! Location within the entire world.
	struct location {
		id region_id;
		tile_hex::point coords;
	};

	//! The hex layout used in Questless.
	constexpr hex_layout world_layout{//
		hex_orientation::flat(),
		view::vector{view::px{30.0}, view::px{36.0} / gcem::sqrt(3.0f)},
		view::point{view::px{0.0}, view::px{0.0}}};

	// Conversions between world and region tile coordinates.

	//! Converts @p p to a view space point.
	constexpr auto to_view_space(tile_hex::point p) {
		auto const x = (world_layout.orientation.f0 * p.q.value + world_layout.orientation.f1 * p.r.value) *
				world_layout.size[0] +
			world_layout.origin[0];
		auto const y = (world_layout.orientation.f2 * p.q.value + world_layout.orientation.f3 * p.r.value) *
				world_layout.size[1] +
			world_layout.origin[1];
		return view::point{x, y};
	}
	//! Converts @p v to a world space vector.
	constexpr auto to_view_space(tile_hex::vector v) {
		return view::point{
			(world_layout.orientation.f0 * v.q.value + world_layout.orientation.f1 * v.r.value) * world_layout.size[0],
			(world_layout.orientation.f2 * v.q.value + world_layout.orientation.f3 * v.r.value) * world_layout.size[1]};
	}

	//! Converts @p p to a region tile space point.
	constexpr auto to_region_tile(view::point p) {
		auto const r0 = world_layout.orientation.b0 * (p[0] - world_layout.origin[0]) / world_layout.size[0];
		auto const r1 = world_layout.orientation.b1 * (p[1] - world_layout.origin[1]) / world_layout.size[1];
		auto const r2 = world_layout.orientation.b2 * (p[0] - world_layout.origin[0]) / world_layout.size[0];
		auto const r3 = world_layout.orientation.b3 * (p[1] - world_layout.origin[1]) / world_layout.size[1];
		return tile_hex::point{span{static_cast<int>((r0 + r1).value)}, span{static_cast<int>((r2 + r3).value)}};
	}
	//! Converts @p v to a region tile space vector.
	constexpr auto to_region_tile(view::vector v) {
		auto const r0 = world_layout.orientation.b0 * v[0] / world_layout.size[0];
		auto const r1 = world_layout.orientation.b1 * v[1] / world_layout.size[1];
		auto const r2 = world_layout.orientation.b2 * v[0] / world_layout.size[0];
		auto const r3 = world_layout.orientation.b3 * v[1] / world_layout.size[1];
		return tile_hex::vector{span{static_cast<int>((r0 + r1).value)}, span{static_cast<int>((r2 + r3).value)}};
	}
}

// Specialize std::hash.
namespace std {
	template <>
	struct hash<ql::tile_hex::point> {
		size_t operator()(ql::tile_hex::point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::tile_hex::vector> {
		size_t operator()(ql::tile_hex::vector const& v) const {
			return hash_value(v);
		}
	};

	template <>
	struct hash<ql::section_hex::point> {
		size_t operator()(ql::section_hex::point const& p) const {
			return hash_value(p);
		}
	};

	template <>
	struct hash<ql::section_hex::vector> {
		size_t operator()(ql::section_hex::vector const& v) const {
			return hash_value(v);
		}
	};
}
