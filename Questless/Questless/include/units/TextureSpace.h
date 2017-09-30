//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace units
{
	namespace detail
	{
		class TextureSpaceBuffer : public BaseBuffer<int, 2>
		{
		public:
			int& u() & { return _elements[0]; }
			constexpr int const& u() const& { return _elements[0]; }

			int& v() & { return _elements[1]; }
			constexpr int const& v() const& { return _elements[1]; }
		protected:
			using BaseBuffer::BaseBuffer;
			using BaseBuffer::operator =;
		};
	}

	struct TextureSpace : Space<struct TextureSpaceTag, int, 2, detail::TextureSpaceBuffer>
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

	inline int& width(TextureSpace::Box& box) { return box.size.u(); }
	inline int width(TextureSpace::Box const& box) { return box.size.u(); }

	inline int& height(TextureSpace::Box& box) { return box.size.v(); }
	inline int height(TextureSpace::Box const& box) { return box.size.v(); }

	inline int& left(TextureSpace::Box& box) { return box.position.u(); }
	inline int left(TextureSpace::Box const& box) { return box.position.u(); }

	inline int& top(TextureSpace::Box& box) { return box.position.v(); }
	inline int top(TextureSpace::Box const& box) { return box.position.v(); }

	inline int right(TextureSpace::Box const& box) { return box.position.u() + box.size.u(); }

	inline int bottom(TextureSpace::Box const& box) { return box.position.v() + box.size.v(); }

	inline TextureSpace::Point center(TextureSpace::Box const& box) { return box.position + box.size / 2; }
}
