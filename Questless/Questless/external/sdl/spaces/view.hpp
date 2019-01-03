//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vecx/box.hpp"
#include "vecx/point.hpp"
#include "vecx/polygon.hpp"
#include "vecx/sphere.hpp"
#include "vecx/vector.hpp"

#include "vecx/define_macros.hpp"

namespace sdl::spaces::view {
	using length = cancel::quantity<float, cancel::unit_t<struct distance_tag>>;

	namespace literals {
		constexpr auto operator "" _view_length(long double value) { return length{static_cast<float>(value)}; }
	}
	using namespace literals;

	using vector = vecx::vector<length, 2>;
	using point = vecx::point<length, 2>;

	DEFINE_VECTOR_INDEX_NAME(vector, 0, x);
	DEFINE_VECTOR_INDEX_NAME(vector, 1, y);
	DEFINE_VECTOR_INDEX_NAME(point, 0, x);
	DEFINE_VECTOR_INDEX_NAME(point, 1, y);

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

	constexpr auto top_left(box const& box) { return box.position; }
	constexpr auto top_right(box const& box) { return point{x(box.position) + x(box.size), y(box.position)}; }
	constexpr auto bottom_left(box const& box) { return point{x(box.position), y(box.position) + y(box.size)}; }
	constexpr auto bottom_right(box const& box) { return box.position + box.size; }
	constexpr auto center(box const& box) { return box.position + box.size / 2; }

	// Circle

	using circle = vecx::sphere<length, 2>;

	// Polygon

	using polygon = vecx::polygon<length>;
}

#include "vecx/undef_macros.hpp"
