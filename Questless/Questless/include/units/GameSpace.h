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
}
