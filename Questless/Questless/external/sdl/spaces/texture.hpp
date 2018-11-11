//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vecx/box.hpp"
#include "vecx/point.hpp"
#include "vecx/sphere.hpp"
#include "vecx/vector.hpp"

#include "vecx/define_macros.hpp"

namespace sdl::spaces::texture {
	//! Texel.
	using tx = cancel::quantity<int, cancel::unit_t<struct tx_tag>>;
	namespace literals {
		constexpr tx operator "" _tx(unsigned long long value) { return tx{static_cast<int>(value)}; }
	}
	using namespace literals;

	using vector = vecx::vector<tx, 2>;
	using point = vecx::point<tx, 2>;

	DEFINE_VECTOR_INDEX_NAME(vector, 0, u);
	DEFINE_VECTOR_INDEX_NAME(vector, 1, v);
	DEFINE_VECTOR_INDEX_NAME(point, 0, u);
	DEFINE_VECTOR_INDEX_NAME(point, 1, v);

	// Box

	using box = vecx::box<tx, 2>;

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
	constexpr auto top_right(box const& box) { return point{u(box.position) + u(box.size), v(box.position)}; }
	constexpr auto bottom_left(box const& box) { return point{u(box.position), v(box.position) + v(box.size)}; }
	constexpr auto bottom_right(box const& box) { return box.position + box.size; }

	// Circle

	using circle = vecx::sphere<tx, 2>;
}

#include "vecx/undef_macros.hpp"
