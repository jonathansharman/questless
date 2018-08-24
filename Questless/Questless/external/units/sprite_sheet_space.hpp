//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace units {
	namespace detail {
		class sprite_sheet_space_buffer : public buffer_base<int, 2> {
		public:
			int& x() & { return _elements[0]; }
			constexpr int const& x() const& { return _elements[0]; }

			int& y() & { return _elements[1]; }
			constexpr int const& y() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	using sprite_sheet_space = space<struct sprite_sheet_space_tag, int, 2, detail::sprite_sheet_space_buffer>;

	inline int& width(sprite_sheet_space::box& box) { return box.size.x(); }
	inline int width(sprite_sheet_space::box const& box) { return box.size.x(); }

	inline int& height(sprite_sheet_space::box& box) { return box.size.y(); }
	inline int height(sprite_sheet_space::box const& box) { return box.size.y(); }

	inline int& left(sprite_sheet_space::box& box) { return box.position.x(); }
	inline int left(sprite_sheet_space::box const& box) { return box.position.x(); }

	inline int& top(sprite_sheet_space::box& box) { return box.position.y(); }
	inline int top(sprite_sheet_space::box const& box) { return box.position.y(); }

	inline int right(sprite_sheet_space::box const& box) { return box.position.x() + box.size.x(); }

	inline int bottom(sprite_sheet_space::box const& box) { return box.position.y() + box.size.y(); }

	inline sprite_sheet_space::point top_left(sprite_sheet_space::box const& box) { return box.position; }
	inline sprite_sheet_space::point top_right(sprite_sheet_space::box const& box) { return sprite_sheet_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline sprite_sheet_space::point bottom_left(sprite_sheet_space::box const& box) { return sprite_sheet_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline sprite_sheet_space::point bottom_right(sprite_sheet_space::box const& box) { return box.position + box.size; }

	inline sprite_sheet_space::point center(sprite_sheet_space::box const& box) { return box.position + box.size / 2; }

	inline sprite_sheet_space::scalar area(sprite_sheet_space::box const& box) { return width(box) * height(box); }
}
