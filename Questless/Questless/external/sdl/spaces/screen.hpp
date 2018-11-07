//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/vector.hpp"

#include <vector>

namespace sdl::spaces::screen {
	using screen_px = meta::quantity<int, meta::unit_t<struct screen_px_tag>>;
	namespace literals {
		constexpr screen_px operator "" _screen_px(unsigned long long value) { return screen_px{static_cast<int>(value)}; }
	}
	using namespace literals;

	constexpr auto& x() { return _elements[0]; }
	constexpr auto const& x() const { return _elements[0]; }

	constexpr auto& y() { return _elements[1]; }
	constexpr auto const& y() const { return _elements[1]; }

	struct screen_space : space<struct screen_space_tag, screen_px, 2, detail::screen_space_buffer> {
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;

		//! Define a polygon as a sequence of its vertex points.
		using polygon = std::vector<point>;
	};

	inline auto& width(screen_space::box& box) { return box.size.x(); }
	inline auto width(screen_space::box const& box) { return box.size.x(); }

	inline auto& height(screen_space::box& box) { return box.size.y(); }
	inline auto height(screen_space::box const& box) { return box.size.y(); }

	inline auto& left(screen_space::box& box) { return box.position.x(); }
	inline auto left(screen_space::box const& box) { return box.position.x(); }

	inline auto& top(screen_space::box& box) { return box.position.y(); }
	inline auto top(screen_space::box const& box) { return box.position.y(); }

	inline auto right(screen_space::box const& box) { return box.position.x() + box.size.x(); }

	inline auto bottom(screen_space::box const& box) { return box.position.y() + box.size.y(); }

	inline auto top_left(screen_space::box const& box) { return box.position; }
	inline auto top_right(screen_space::box const& box) { return screen_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline auto bottom_left(screen_space::box const& box) { return screen_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline auto bottom_right(screen_space::box const& box) { return box.position + box.size; }

	inline auto center(screen_space::box const& box) { return box.position + box.size / 2; }

	inline auto area(screen_space::box const& box) { return width(box) * height(box); }
}
