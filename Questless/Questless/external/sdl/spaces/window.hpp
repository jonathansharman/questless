//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "view.hpp" // for conversions

namespace sdl::spaces::window {
	using px = meta::quantity<int, meta::unit_t<struct px_tag>>;
	namespace literals {
		constexpr px operator "" _px(unsigned long long value) { return px{static_cast<int>(value)}; }
	}
	using namespace literals;

	constexpr auto& x() { return _elements[0]; }
	constexpr auto const& x() const { return _elements[0]; }

	constexpr auto& y() { return _elements[1]; }
	constexpr auto const& y() const { return _elements[1]; }

	struct window_space : space<struct window_space_tag, px, 2, detail::window_space_buffer> {
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;
	};

	inline px& width(window_space::box& box) { return box.size.x(); }
	inline px width(window_space::box const& box) { return box.size.x(); }

	inline px& height(window_space::box& box) { return box.size.y(); }
	inline px height(window_space::box const& box) { return box.size.y(); }

	inline px& left(window_space::box& box) { return box.position.x(); }
	inline px left(window_space::box const& box) { return box.position.x(); }

	inline px& top(window_space::box& box) { return box.position.y(); }
	inline px top(window_space::box const& box) { return box.position.y(); }

	inline px right(window_space::box const& box) { return box.position.x() + box.size.x(); }

	inline px bottom(window_space::box const& box) { return box.position.y() + box.size.y(); }

	inline auto top_left(window_space::box const& box) { return box.position; }
	inline auto top_right(window_space::box const& box) { return window_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline auto bottom_left(window_space::box const& box) { return window_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline auto bottom_right(window_space::box const& box) { return box.position + box.size; }

	inline auto center(window_space::box const& box) { return box.position + box.size / 2; }

	inline auto area(window_space::box const& box) { return width(box) * height(box); }

	//! @todo Add a uniform mechanism for converting between spaces?

	static constexpr auto px_per_view = 1_px / 1.0_view;
	static constexpr auto view_per_px = 1.0_view / 1_px;

	inline view to_view_space(px s) {
		return s * view_per_px;
	}

	inline view_space::point to_view_space(window_space::point p) {
		return {to_view_space(p.x()), to_view_space(p.y())};
	}

	inline view_space::vector to_view_space(window_space::vector v) {
		return {v.x() * view_per_px, v.y() * view_per_px};
	}

	inline view_space::box to_view_space(window_space::box b) {
		return {to_view_space(b.position), to_view_space(b.size)};
	}

	inline view_space::sphere to_view_space(window_space::sphere s) {
		return {to_view_space(s.center), to_view_space(s.radius)};
	}
}
