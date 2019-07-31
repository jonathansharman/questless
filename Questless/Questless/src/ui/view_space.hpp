//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vecx/box.hpp"
#include "vecx/point.hpp"
#include "vecx/polygon.hpp"
#include "vecx/sphere.hpp"
#include "vecx/vector.hpp"

#include <SFML/Graphics.hpp>

#include "vecx/define_macros.hpp"

namespace ql::view {
	using px = cancel::quantity<float, cancel::unit_t<struct distance_tag>>;

	namespace literals {
		constexpr auto operator"" _px(long double value) {
			return px{static_cast<float>(value)};
		}
	}
	using namespace literals;

	using vector = vecx::vector<px, 2>;
	using point = vecx::point<px, 2>;

	DEFINE_VECTOR_INDEX_NAME(vector, 0, x);
	DEFINE_VECTOR_INDEX_NAME(vector, 1, y);
	DEFINE_VECTOR_INDEX_NAME(point, 0, x);
	DEFINE_VECTOR_INDEX_NAME(point, 1, y);

	// Box

	using box = vecx::box<px, 2>;

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

	using circle = vecx::sphere<px, 2>;

	// Polygon

	using polygon = vecx::polygon<px>;

	/////////////////////////////////////////////////////////
	// Conversions between view space types and SFML types //
	/////////////////////////////////////////////////////////

	//! Converts from a @p view::vector into an @p sf::Vector.
	sf::Vector2f to_sfml(vector v) {
		return {v[0].value, v[1].value};
	}

	//! Converts from a @p view::point into an @p sf::Vector.
	sf::Vector2f to_sfml(point p) {
		return {p[0].value, p[1].value};
	}

	// Conversions from SFML to view space

	//! Converts from an @p sf::Vector to a @p view::vector.
	template <typename T>
	auto vector_from_sfml(sf::Vector2<T> const& v) {
		return vector{px{static_cast<px::rep>(v.x)}, px{static_cast<px::rep>(v.y)}};
	}

	//! Converts from an sf::SizeEvent to a view::vector.
	auto vector_from_size_event(sf::Event::SizeEvent size_event) {
		return vector{px{static_cast<px::rep>(size_event.width)}, px{static_cast<px::rep>(size_event.height)}};
	}

	//! Converts the size of an sf::Rect<T> to a view::vector.
	template <typename T>
	auto vector_from_sfml_rect(sf::Rect<T> rect) {
		return vector{px{static_cast<px::rep>(rect.width)}, px{static_cast<px::rep>(rect.height)}};
	}

	//! Converts from an @p sf::Vector to a @p view::point.
	template <typename T>
	auto point_from_sfml(sf::Vector2<T> const& p) {
		return point{px{static_cast<px::rep>(p.x)}, px{static_cast<px::rep>(p.y)}};
	}

	//! Extracts the size from an @p sf::Rect as a @p view::vector.
	template <typename T>
	auto get_size(sf::Rect<T> const& r) {
		return vector{px{static_cast<px::rep>(r.width)}, px{static_cast<px::rep>(r.height)}};
	}

	//! Extracts the width from an @p sf::Vector as a @p view::px.
	template <typename T>
	auto get_width(sf::Vector2<T> const& v) {
		return px{static_cast<px::rep>(v.x)};
	}

	//! Extracts the height from an @p sf::Vector as a @p view::px.
	template <typename T>
	auto get_height(sf::Vector2<T> const& v) {
		return px{static_cast<px::rep>(v.y)};
	}
}

#include "vecx/undef_macros.hpp"
