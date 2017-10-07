//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Space.h"

namespace units
{
	namespace detail
	{
		class ViewSpaceBuffer : public BaseBuffer<float, 2>
		{
		public:
			auto& x() & { return _elements[0]; }
			constexpr auto const& x() const& { return _elements[0]; }

			auto& y() & { return _elements[1]; }
			constexpr auto const& y() const& { return _elements[1]; }
		protected:
			using BaseBuffer::BaseBuffer;
			using BaseBuffer::operator =;
		};
	}

	struct ViewSpace : Space<struct ViewSpaceTag, float, 2, detail::ViewSpaceBuffer>
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

	inline auto& width(ViewSpace::Box& box) { return box.size.x(); }
	inline auto width(ViewSpace::Box const& box) { return box.size.x(); }

	inline auto& height(ViewSpace::Box& box) { return box.size.y(); }
	inline auto height(ViewSpace::Box const& box) { return box.size.y(); }

	inline auto& left(ViewSpace::Box& box) { return box.position.x(); }
	inline auto left(ViewSpace::Box const& box) { return box.position.x(); }

	inline auto& top(ViewSpace::Box& box) { return box.position.y(); }
	inline auto top(ViewSpace::Box const& box) { return box.position.y(); }

	inline auto right(ViewSpace::Box const& box) { return box.position.x() + box.size.x(); }

	inline auto bottom(ViewSpace::Box const& box) { return box.position.y() + box.size.y(); }

	inline ViewSpace::Point top_left(ViewSpace::Box const& box) { return box.position; }
	inline ViewSpace::Point top_right(ViewSpace::Box const& box) { return ViewSpace::Point{box.position.x() + box.size.x(), box.position.y()}; }
	inline ViewSpace::Point bottom_left(ViewSpace::Box const& box) { return ViewSpace::Point{box.position.x(), box.position.y() + box.size.y()}; }
	inline ViewSpace::Point bottom_right(ViewSpace::Box const& box) { return box.position + box.size; }

	inline ViewSpace::Point center(ViewSpace::Box const& box) { return box.position + box.size / 2; }
}
