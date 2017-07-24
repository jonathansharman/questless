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

	using GameSpace = Space<struct GameSpaceTag, double, 2, detail::GameSpaceBuffer>;

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
}
