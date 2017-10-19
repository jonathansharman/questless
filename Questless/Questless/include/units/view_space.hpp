//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "space.hpp"

namespace units
{
	namespace detail
	{
		class view_space_buffer : public buffer_base<float, 2>
		{
		public:
			auto& x() & { return _elements[0]; }
			constexpr auto const& x() const& { return _elements[0]; }

			auto& y() & { return _elements[1]; }
			constexpr auto const& y() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	struct view_space : space<struct view_space_tag, float, 2, detail::view_space_buffer>
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

	inline view_space::point top_left(view_space::box const& box) { return box.position; }
	inline view_space::point top_right(view_space::box const& box) { return view_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline view_space::point bottom_left(view_space::box const& box) { return view_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline view_space::point bottom_right(view_space::box const& box) { return box.position + box.size; }

	inline view_space::point center(view_space::box const& box) { return box.position + box.size / 2; }
}
