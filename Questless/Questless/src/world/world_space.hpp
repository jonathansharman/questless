//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines the world space and related units.

#pragma once

#include "quantities/wall_time.hpp"

#include "vecx/angle.hpp"
#include "vecx/box.hpp"
#include "vecx/point.hpp"
#include "vecx/sphere.hpp"
#include "vecx/vector.hpp"

#include "vecx/define_macros.hpp"

#include <SFML/System.hpp>

namespace ql::world {
	using length = cancel::quantity<float, cancel::unit_t<struct distance_tag>>;

	namespace literals {
		constexpr auto operator"" _world_length(long double value) {
			return length{static_cast<float>(value)};
		}
	}
	using namespace literals;

	using vector = vecx::vector<length, 2>;
	using point = vecx::point<length, 2>;

	DEFINE_VECTOR_INDEX_NAME(vector, 0, x);
	DEFINE_VECTOR_INDEX_NAME(vector, 1, y);
	DEFINE_VECTOR_INDEX_NAME(point, 0, x);
	DEFINE_VECTOR_INDEX_NAME(point, 1, y);

	// Timing and Rates

	using speed = cancel::quotient_t<length, sec>;
	using vel = decltype(vector{} / sec{});
	using accel = decltype(vel{} / sec{});
	using angular_vel = cancel::quotient_t<vecx::radians, sec>;

	// Box

	using box = vecx::box<length, 2>;

	using h_align = box::axis<0>::align_t;
	using v_align = box::axis<1>::align_t;

	constexpr auto left_align = h_align::near;
	constexpr auto center_align = h_align::mid;
	constexpr auto right_align = h_align::far;

	constexpr auto top_align = v_align::near;
	constexpr auto middle_align = v_align::mid;
	constexpr auto bottom_align = v_align::far;

	DEFINE_BOX_SIZE_NAME(box, 0, width);
	DEFINE_BOX_SIZE_NAME(box, 1, height);

	DEFINE_BOX_EXTREMES_NAMES(box, 0, left, right);
	DEFINE_BOX_EXTREMES_NAMES(box, 1, top, bottom);

	constexpr auto top_left(box const& box) {
		return box.position;
	}
	constexpr auto top_right(box const& box) {
		return point{x(box.position) + x(box.size), y(box.position)};
	}
	constexpr auto bottom_left(box const& box) {
		return point{x(box.position), y(box.position) + y(box.size)};
	}
	constexpr auto bottom_right(box const& box) {
		return box.position + box.size;
	}
	constexpr auto center(box const& box) {
		return box.position + box.size / 2;
	}

	// Circle

	using circle = vecx::sphere<length, 2>;

	// Conversions from world space to SFML

	sf::Vector2f to_sfml(vector v) {
		return {v[0].value, v[1].value};
	}

	sf::Vector2f to_sfml(point p) {
		return {p[0].value, p[1].value};
	}

	// Conversions from SFML to world space

	template <typename T>
	vector vector_from_sfml(sf::Vector2<T> const& v) {
		return {length{v.x}, length{v.y}};
	}

	template <typename T>
	point point_from_sfml(sf::Vector2<T> const& p) {
		return {length{p.x}, length{p.y}};
	}
}

#include "vecx/undef_macros.hpp"
