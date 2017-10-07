//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Space.h"

// For conversions.
#include "ViewSpace.h"

namespace units
{
	namespace detail
	{
		class ScreenSpaceBuffer : public BaseBuffer<int, 2>
		{
		public:
			int& x() & { return _elements[0]; }
			constexpr int const& x() const& { return _elements[0]; }

			int& y() & { return _elements[1]; }
			constexpr int const& y() const& { return _elements[1]; }
		protected:
			using BaseBuffer::BaseBuffer;
			using BaseBuffer::operator =;
		};
	}

	struct ScreenSpace : Space<struct ScreenSpaceTag, int, 2, detail::ScreenSpaceBuffer>
	{
		using HAlign = Axis<0>::Align;
		using VAlign = Axis<1>::Align;

		static constexpr auto align_left = HAlign::near;
		static constexpr auto align_center = HAlign::mid;
		static constexpr auto align_right = HAlign::far;

		static constexpr auto align_top = VAlign::near;
		static constexpr auto align_middle = VAlign::mid;
		static constexpr auto align_bottom = VAlign::far;
	};

	inline int& width(ScreenSpace::Box& box) { return box.size.x(); }
	inline int width(ScreenSpace::Box const& box) { return box.size.x(); }

	inline int& height(ScreenSpace::Box& box) { return box.size.y(); }
	inline int height(ScreenSpace::Box const& box) { return box.size.y(); }

	inline int& left(ScreenSpace::Box& box) { return box.position.x(); }
	inline int left(ScreenSpace::Box const& box) { return box.position.x(); }

	inline int& top(ScreenSpace::Box& box) { return box.position.y(); }
	inline int top(ScreenSpace::Box const& box) { return box.position.y(); }

	inline int right(ScreenSpace::Box const& box) { return box.position.x() + box.size.x(); }

	inline int bottom(ScreenSpace::Box const& box) { return box.position.y() + box.size.y(); }

	inline ScreenSpace::Point top_left(ScreenSpace::Box const& box) { return box.position; }
	inline ScreenSpace::Point top_right(ScreenSpace::Box const& box) { return ScreenSpace::Point{box.position.x() + box.size.x(), box.position.y()}; }
	inline ScreenSpace::Point bottom_left(ScreenSpace::Box const& box) { return ScreenSpace::Point{box.position.x(), box.position.y() + box.size.y()}; }
	inline ScreenSpace::Point bottom_right(ScreenSpace::Box const& box) { return box.position + box.size; }

	inline ScreenSpace::Point center(ScreenSpace::Box const& box) { return box.position + box.size / 2; }

	//! @todo Add a uniform mechanism for converting between spaces?

	inline ViewSpace::scalar_t to_view_space(ScreenSpace::scalar_t s) { return static_cast<ViewSpace::scalar_t>(s); }
	inline ViewSpace::Point to_view_space(ScreenSpace::Point p) { return ViewSpace::Point{to_view_space(p.x()), to_view_space(p.y())}; }
	inline ViewSpace::Vector to_view_space(ScreenSpace::Vector v) { return ViewSpace::Vector{to_view_space(v.x()), to_view_space(v.y())}; }
	inline ViewSpace::Box to_view_space(ScreenSpace::Box b) { return ViewSpace::Box{to_view_space(b.position), to_view_space(b.size)}; }
}
