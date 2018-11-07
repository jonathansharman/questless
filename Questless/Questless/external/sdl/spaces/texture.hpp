//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/vector.hpp"

namespace sdl::spaces::units {
	using tx = meta::quantity<int, meta::unit_t<struct tx_tag>>;
	namespace literals {
		constexpr tx operator "" _tx(unsigned long long value) { return tx{static_cast<int>(value)}; }
	}
	using namespace literals;

	constexpr auto& u() & { return _elements[0]; }
	constexpr auto const& u() const& { return _elements[0]; }

	constexpr auto& v() & { return _elements[1]; }
	constexpr auto const& v() const& { return _elements[1]; }

	struct texture_space : space<struct texture_space_tag, tx, 2, detail::texture_space_buffer> {
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;
	};

	inline auto& width(texture_space::box& box) { return box.size.u(); }
	inline auto width(texture_space::box const& box) { return box.size.u(); }

	inline auto& height(texture_space::box& box) { return box.size.v(); }
	inline auto height(texture_space::box const& box) { return box.size.v(); }

	inline auto& left(texture_space::box& box) { return box.position.u(); }
	inline auto left(texture_space::box const& box) { return box.position.u(); }

	inline auto& top(texture_space::box& box) { return box.position.v(); }
	inline auto top(texture_space::box const& box) { return box.position.v(); }

	inline auto right(texture_space::box const& box) { return box.position.u() + box.size.u(); }

	inline auto bottom(texture_space::box const& box) { return box.position.v() + box.size.v(); }

	inline auto top_left(texture_space::box const& box) { return box.position; }
	inline auto top_right(texture_space::box const& box) { return texture_space::point{box.position.u() + box.size.u(), box.position.v()}; }
	inline auto bottom_left(texture_space::box const& box) { return texture_space::point{box.position.u(), box.position.v() + box.size.v()}; }
	inline auto bottom_right(texture_space::box const& box) { return box.position + box.size; }

	inline auto center(texture_space::box const& box) { return box.position + box.size / 2; }

	inline auto area(texture_space::box const& box) { return width(box) * height(box); }
}
