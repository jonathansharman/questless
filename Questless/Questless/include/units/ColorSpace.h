//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Space.h"

namespace units
{
	namespace detail
	{
		class ColorBuffer : public BaseBuffer<float, 4>
		{
		public:
			float& red() & { return _elements[0]; }
			constexpr float const& red() const& { return _elements[0]; }

			float& green() & { return _elements[1]; }
			constexpr float const& green() const& { return _elements[1]; }

			float& blue() & { return _elements[2]; }
			constexpr float const& blue() const& { return _elements[2]; }

			float& alpha() & { return _elements[3]; }
			constexpr float const& alpha() const& { return _elements[3]; }
		protected:
			using BaseBuffer::BaseBuffer;
			using BaseBuffer::operator =;
		};
	}

	using ColorSpace = Space<struct ColorSpaceTag, float, 4, detail::ColorBuffer>;
}
