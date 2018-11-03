//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "quantities/quantities.hpp"

#include "units/world_space.hpp"
#include "units/hex_space.hpp"

namespace ql {
	//! The space of tiles in a section.
	using section_tile = units::hex_space<struct section_tile_tag, span>;

	//! The space of tiles in a region.
	using region_tile = units::hex_space<struct region_tile_tag, span>;

	//! The space of sections in a region.
	using region_section = units::hex_space<struct region_section_tag, section_span>;

	//! Location within the entire world.
	struct global_coords {
		std::string region;
		region_section::point section;

		friend bool operator ==(global_coords const& left, global_coords const& right) {
			return left.region == right.region || (left.region == right.region && left.section < right.section);
		}
		friend bool operator <(global_coords const& left, global_coords const& right) {
			return left.region < right.region || (left.region == right.region && left.section < right.section);
		}
	};

	//! The hex layout used in Questless.
	constexpr auto hex_layout = units::hex_layout
		{ units::hex_orientation::flat()
		, units::world_space::vector{30.0f, 36.0f / units::math::sqrt(3.0f)}
		, units::world_space::point{0.0f, 0.0f}
		};

	// Conversions between world and region tile coordinates.

	//! Converts @p p to a game space point.
	constexpr auto to_world(region_tile::point p) {
		return units::world_space::point
			{ ((hex_layout.orientation.f0 * p.q.value + hex_layout.orientation.f1 * p.r.value) * hex_layout.size.x() + hex_layout.origin.x())
			, ((hex_layout.orientation.f2 * p.q.value + hex_layout.orientation.f3 * p.r.value) * hex_layout.size.y() + hex_layout.origin.y())
			};
	}
	//! Converts @p v to a game space vector.
	constexpr auto to_world(region_tile::vector v) {
		return units::world_space::point
			{ ((hex_layout.orientation.f0 * v.q.value + hex_layout.orientation.f1 * v.r.value) * hex_layout.size.x())
			, ((hex_layout.orientation.f2 * v.q.value + hex_layout.orientation.f3 * v.r.value) * hex_layout.size.y())
			};
	}

	//! Converts @p p to a region tile space point.
	constexpr auto to_region_tile(units::world_space::point p) {
		return region_tile::point
			{ hex_layout.orientation.b0 * (p.x() - hex_layout.origin.x()) / hex_layout.size.x() + hex_layout.orientation.b1 * (p.y() - hex_layout.origin.y()) / hex_layout.size.y()
			, hex_layout.orientation.b2 * (p.x() - hex_layout.origin.x()) / hex_layout.size.x() + hex_layout.orientation.b3 * (p.y() - hex_layout.origin.y()) / hex_layout.size.y()
			};
	}
	//! Converts @p v to a region tile space vector.
	constexpr auto to_region_tile(units::world_space::vector v) {
		return region_tile::point
			{ hex_layout.orientation.b0 * v.x() / hex_layout.size.x() + hex_layout.orientation.b1 * v.y() / hex_layout.size.y()
			, hex_layout.orientation.b2 * v.x() / hex_layout.size.x() + hex_layout.orientation.b3 * v.y() / hex_layout.size.y()
			};
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
