//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "space.hpp"

namespace units
{
	namespace detail
	{
		class screen_space_buffer : public buffer_base<int, 2>
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

	struct screen_space : space<struct screen_space_tag, int, 2, detail::screen_space_buffer>
	{
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

	inline int& width(screen_space::box& box) { return box.size.x(); }
	inline int width(screen_space::box const& box) { return box.size.x(); }

	inline int& height(screen_space::box& box) { return box.size.y(); }
	inline int height(screen_space::box const& box) { return box.size.y(); }

	inline int& left(screen_space::box& box) { return box.position.x(); }
	inline int left(screen_space::box const& box) { return box.position.x(); }

	inline int& top(screen_space::box& box) { return box.position.y(); }
	inline int top(screen_space::box const& box) { return box.position.y(); }

	inline int right(screen_space::box const& box) { return box.position.x() + box.size.x(); }

	inline int bottom(screen_space::box const& box) { return box.position.y() + box.size.y(); }

	inline screen_space::point top_left(screen_space::box const& box) { return box.position; }
	inline screen_space::point top_right(screen_space::box const& box) { return screen_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline screen_space::point bottom_left(screen_space::box const& box) { return screen_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline screen_space::point bottom_right(screen_space::box const& box) { return box.position + box.size; }

	inline screen_space::point center(screen_space::box const& box) { return box.position + box.size / 2; }

	inline screen_space::scalar area(screen_space::box const& box) { return width(box) * height(box); }
}
