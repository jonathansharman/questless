//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/box.hpp"
#include "units/point.hpp"
#include "units/vector.hpp"

namespace sdl::spaces::view {
	using distance = meta::quantity<float, meta::unit_t<struct distance_tag>>;

	namespace literals {
		constexpr auto operator "" _view_distance(long double value) { return distance{static_cast<double>(value)}; }
	}
	using namespace literals;

	using vector = units::vector<distance, 2>;
	using point = units::point<distance, 2>;

	constexpr auto& x(vector& v) { return v[0]; }
	constexpr auto const& x(vector const& v) { return v[0]; }

	constexpr auto& y(vector& v) { return v[1]; }
	constexpr auto const& y(vector const& v) { return v[1]; }

	constexpr auto& x(point& v) { return v[0]; }
	constexpr auto const& x(point const& v) { return v[0]; }

	constexpr auto& y(point& v) { return v[1]; }
	constexpr auto const& y(point const& v) { return v[1]; }

	using h_align = axis<0>::align;
	using v_align = axis<1>::align;

	constexpr auto align_left = h_align::near;
	constexpr auto align_center = h_align::mid;
	constexpr auto align_right = h_align::far;

	constexpr auto align_top = v_align::near;
	constexpr auto align_middle = v_align::mid;
	constexpr auto align_bottom = v_align::far;

	// Box Types and Functions

	using box = units::box<distance, distance>;

	constexpr auto& width(box& box) { return x(box.size); }
	constexpr auto width(box const& box) { return x(box.size); }

	constexpr auto& height(box& box) { return y(box.size); }
	constexpr auto height(box const& box) { return y(box.size); }

	constexpr auto& left(box& box) { return x(box.position); }
	constexpr auto left(box const& box) { return x(box.position); }

	constexpr auto& top(box& box) { return y(box.position); }
	constexpr auto top(box const& box) { return y(box.position); }

	constexpr auto right(box const& box) { return x(box.position) + x(box.size); }

	constexpr auto bottom(box const& box) { return y(box.position) + y(box.size); }

	constexpr auto top_left(box const& box) { return box.position; }
	constexpr auto top_right(box const& box) { return point{x(box.position) + x(box.size), y(box.position)}; }
	constexpr auto bottom_left(box const& box) { return point{x(box.position), y(box.position) + y(box.size)}; }
	constexpr auto bottom_right(box const& box) { return box.position + box.size; }

	constexpr auto center(box const& box) { return box.position + box.size / 2.0; }

	constexpr auto area(box const& box) { return width(box) * height(box); }



	using view = meta::quantity<float, meta::unit_t<struct view_tag>>;
	namespace literals {
		constexpr view operator "" _view(long double value) { return view{static_cast<float>(value)}; }
	}
	using namespace literals;

			constexpr auto& x() & { return _elements[0]; }
			constexpr auto const& x() const& { return _elements[0]; }

			constexpr auto& y() & { return _elements[1]; }
			constexpr auto const& y() const& { return _elements[1]; }

	struct view_space : space<struct view_space_tag, view, 2, detail::view_space_buffer> {
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;
	};

	inline auto& width(view_space::box& box) { return box.size.x(); }
	inline auto width(view_space::box const& box) { return box.size.x(); }

	inline auto& height(view_space::box& box) { return box.size.y(); }
	inline auto height(view_space::box const& box) { return box.size.y(); }

	inline auto& left(view_space::box& box) { return box.position.x(); }
	inline auto left(view_space::box const& box) { return box.position.x(); }

	inline auto& top(view_space::box& box) { return box.position.y(); }
	inline auto top(view_space::box const& box) { return box.position.y(); }

	inline auto right(view_space::box const& box) { return box.position.x() + box.size.x(); }

	inline auto bottom(view_space::box const& box) { return box.position.y() + box.size.y(); }

	inline auto top_left(view_space::box const& box) { return box.position; }
	inline auto top_right(view_space::box const& box) { return view_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline auto bottom_left(view_space::box const& box) { return view_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline auto bottom_right(view_space::box const& box) { return box.position + box.size; }

	inline auto center(view_space::box const& box) { return box.position + box.size / 2.0f; }

	inline auto area(view_space::box const& box) { return width(box) * height(box); }
}
