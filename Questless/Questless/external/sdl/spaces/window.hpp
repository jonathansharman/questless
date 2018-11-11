//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "view.hpp" // for conversions

#include <range/v3/algorithm/transform.hpp>

#include "vecx/define_macros.hpp"

namespace sdl::spaces::window {
	//! Window pixel.
	using px = cancel::quantity<int, cancel::unit_t<struct px_tag>>;
	namespace literals {
		constexpr px operator "" _px(unsigned long long value) { return px{static_cast<int>(value)}; }
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

	using h_align = box::align<0>;
	using v_align = box::align<1>;

	constexpr auto left_align = h_align{h_align::near};
	constexpr auto center_align = h_align{h_align::mid};
	constexpr auto right_align = h_align{h_align::far};

	constexpr auto top_align = v_align{v_align::near};
	constexpr auto middle_align = v_align{v_align::mid};
	constexpr auto bottom_align = v_align{v_align::far};

	DEFINE_BOX_SIZE_NAME(box, 0, width);
	DEFINE_BOX_SIZE_NAME(box, 1, height);

	DEFINE_BOX_EXTREMES_NAMES(box, 0, left, right);
	DEFINE_BOX_EXTREMES_NAMES(box, 1, top, bottom);

	constexpr auto top_left(box const& box) { return box.position; }
	constexpr auto top_right(box const& box) { return point{x(box.position) + x(box.size), y(box.position)}; }
	constexpr auto bottom_left(box const& box) { return point{x(box.position), y(box.position) + y(box.size)}; }
	constexpr auto bottom_right(box const& box) { return box.position + box.size; }

	// Circle

	using circle = vecx::sphere<px, 2>;

	// Polygon

	using polygon = vecx::polygon<px>;

	// View Conversions

	using namespace view::literals;

	static constexpr auto px_per_view_length = 1_px / 1.0_view_length;
	static constexpr auto view_length_per_px = 1.0_view_length / 1_px;

	inline auto to_view_space(px s) {
		return s * view_length_per_px;
	}

	inline auto to_view_space(point p) {
		return view::point{to_view_space(x(p)), to_view_space(y(p))};
	}

	inline auto to_view_space(vector v) {
		return view::vector{to_view_space(x(v)), to_view_space(y(v))};
	}

	inline auto to_view_space(box b) {
		return view::box{to_view_space(b.position), to_view_space(b.size)};
	}

	inline auto to_view_space(circle c) {
		return view::circle{to_view_space(c.center), to_view_space(c.radius)};
	}

	inline auto to_view_space(polygon polygon) {
		view::polygon result;
		for (auto const& p : polygon.vertices) {
			result.vertices.push_back(to_view_space(p));
		}
		return result;
	}
}

#include "vecx/undef_macros.hpp"
