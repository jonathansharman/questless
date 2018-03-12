//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "space.hpp"

#include "view_space.hpp" // for conversions

namespace units
{
	namespace detail
	{
		class window_space_buffer : public buffer_base<int, 2>
		{
		public:
			int& x() & { return _elements[0]; }
			constexpr int const& x() const& { return _elements[0]; }

			int& y() & { return _elements[1]; }
			constexpr int const& y() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	struct window_space : space<struct window_space_tag, int, 2, detail::window_space_buffer>
	{
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;
	};

	inline int& width(window_space::box& box) { return box.size.x(); }
	inline int width(window_space::box const& box) { return box.size.x(); }

	inline int& height(window_space::box& box) { return box.size.y(); }
	inline int height(window_space::box const& box) { return box.size.y(); }

	inline int& left(window_space::box& box) { return box.position.x(); }
	inline int left(window_space::box const& box) { return box.position.x(); }

	inline int& top(window_space::box& box) { return box.position.y(); }
	inline int top(window_space::box const& box) { return box.position.y(); }

	inline int right(window_space::box const& box) { return box.position.x() + box.size.x(); }

	inline int bottom(window_space::box const& box) { return box.position.y() + box.size.y(); }

	inline window_space::point top_left(window_space::box const& box) { return box.position; }
	inline window_space::point top_right(window_space::box const& box) { return window_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline window_space::point bottom_left(window_space::box const& box) { return window_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline window_space::point bottom_right(window_space::box const& box) { return box.position + box.size; }

	inline window_space::point center(window_space::box const& box) { return box.position + box.size / 2; }

	inline window_space::scalar area(window_space::box const& box) { return width(box) * height(box); }

	//! @todo Add a uniform mechanism for converting between spaces?

	inline view_space::scalar to_view_space(window_space::scalar s) { return static_cast<view_space::scalar>(s); }
	inline view_space::point to_view_space(window_space::point p) { return view_space::point{to_view_space(p.x()), to_view_space(p.y())}; }
	inline view_space::vector to_view_space(window_space::vector v) { return view_space::vector{to_view_space(v.x()), to_view_space(v.y())}; }
	inline view_space::box to_view_space(window_space::box b) { return view_space::box{to_view_space(b.position), to_view_space(b.size)}; }
	inline view_space::sphere to_view_space(window_space::sphere s) { return view_space::sphere{to_view_space(s.center), to_view_space(s.radius)}; }
}
