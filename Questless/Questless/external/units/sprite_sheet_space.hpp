//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "space.hpp"

namespace units {
	using cel = meta::quantity<int, meta::unit_t<struct cel_tag>>;
	namespace literals {
		constexpr cel operator "" _cel(unsigned long long value) { return cel{static_cast<int>(value)}; }
	}
	using namespace literals;

	namespace detail {
		class sprite_sheet_space_buffer : public buffer_base<cel, 2> {
		public:
			constexpr auto& x() & { return _elements[0]; }
			constexpr auto const& x() const& { return _elements[0]; }

			constexpr auto& y() & { return _elements[1]; }
			constexpr auto const& y() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	using sprite_sheet_space = space<struct sprite_sheet_space_tag, cel, 2, detail::sprite_sheet_space_buffer>;

	inline auto& width(sprite_sheet_space::box& box) { return box.size.x(); }
	inline auto width(sprite_sheet_space::box const& box) { return box.size.x(); }

	inline auto& height(sprite_sheet_space::box& box) { return box.size.y(); }
	inline auto height(sprite_sheet_space::box const& box) { return box.size.y(); }

	inline auto& left(sprite_sheet_space::box& box) { return box.position.x(); }
	inline auto left(sprite_sheet_space::box const& box) { return box.position.x(); }

	inline auto& top(sprite_sheet_space::box& box) { return box.position.y(); }
	inline auto top(sprite_sheet_space::box const& box) { return box.position.y(); }

	inline auto right(sprite_sheet_space::box const& box) { return box.position.x() + box.size.x(); }

	inline auto bottom(sprite_sheet_space::box const& box) { return box.position.y() + box.size.y(); }

	inline auto top_left(sprite_sheet_space::box const& box) { return box.position; }
	inline auto top_right(sprite_sheet_space::box const& box) { return sprite_sheet_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline auto bottom_left(sprite_sheet_space::box const& box) { return sprite_sheet_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline auto bottom_right(sprite_sheet_space::box const& box) { return box.position + box.size; }

	inline auto center(sprite_sheet_space::box const& box) { return box.position + box.size / 2; }

	inline auto area(sprite_sheet_space::box const& box) { return width(box) * height(box); }
}
