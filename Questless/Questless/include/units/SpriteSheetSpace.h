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

	inline int& width(SpriteSheetSpace::Box& box) { return box.size.x(); }
	inline int width(SpriteSheetSpace::Box const& box) { return box.size.x(); }

	inline int& height(SpriteSheetSpace::Box& box) { return box.size.y(); }
	inline int height(SpriteSheetSpace::Box const& box) { return box.size.y(); }

	inline int& left(SpriteSheetSpace::Box& box) { return box.position.x(); }
	inline int left(SpriteSheetSpace::Box const& box) { return box.position.x(); }

	inline int& top(SpriteSheetSpace::Box& box) { return box.position.y(); }
	inline int top(SpriteSheetSpace::Box const& box) { return box.position.y(); }

	inline int right(SpriteSheetSpace::Box const& box) { return box.position.x() + box.size.x(); }

	inline int bottom(SpriteSheetSpace::Box const& box) { return box.position.y() + box.size.y(); }

	inline SpriteSheetSpace::Point center(SpriteSheetSpace::Box const& box) { return box.position + box.size / 2; }
}
