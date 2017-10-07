//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Space.h"

namespace units
{
	namespace detail
	{
		class GameSpaceBuffer : public BaseBuffer<double, 2>
		{
		public:
			double& x() & { return _elements[0]; }
			constexpr double const& x() const& { return _elements[0]; }

			double& y() & { return _elements[1]; }
			constexpr double const& y() const& { return _elements[1]; }
		protected:
			using BaseBuffer::BaseBuffer;
			using BaseBuffer::operator =;
		};
	}

	struct GameSpace : Space<struct GameSpaceTag, double, 2, detail::GameSpaceBuffer>
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

	inline double& width(GameSpace::Box& box) { return box.size.x(); }
	inline double width(GameSpace::Box const& box) { return box.size.x(); }

	inline double& height(GameSpace::Box& box) { return box.size.y(); }
	inline double height(GameSpace::Box const& box) { return box.size.y(); }

	inline double& left(GameSpace::Box& box) { return box.position.x(); }
	inline double left(GameSpace::Box const& box) { return box.position.x(); }

	inline double& top(GameSpace::Box& box) { return box.position.y(); }
	inline double top(GameSpace::Box const& box) { return box.position.y(); }

	inline double right(GameSpace::Box const& box) { return box.position.x() + box.size.x(); }

	inline double bottom(GameSpace::Box const& box) { return box.position.y() + box.size.y(); }

	inline GameSpace::Point top_left(GameSpace::Box const& box) { return box.position; }
	inline GameSpace::Point top_right(GameSpace::Box const& box) { return GameSpace::Point{box.position.x() + box.size.x(), box.position.y()}; }
	inline GameSpace::Point bottom_left(GameSpace::Box const& box) { return GameSpace::Point{box.position.x(), box.position.y() + box.size.y()}; }
	inline GameSpace::Point bottom_right(GameSpace::Box const& box) { return box.position + box.size; }

	inline GameSpace::Point center(GameSpace::Box const& box) { return box.position + box.size / 2.0; }
}
