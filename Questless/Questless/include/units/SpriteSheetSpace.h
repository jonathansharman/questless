//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace units
{
	namespace detail
	{
		class SpriteSheetSpaceBuffer : public BaseBuffer<int, 2>
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

	using SpriteSheetSpace = Space<struct SpriteSheetSpaceTag, int, 2, detail::SpriteSheetSpaceBuffer>;
}
