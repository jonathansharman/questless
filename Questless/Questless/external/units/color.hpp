//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vector.hpp"

namespace units {
	using red_part = meta::quantity<float, meta::unit_t<struct red_part_tag>>;
	using green_part = meta::quantity<float, meta::unit_t<struct green_part_tag>>;
	using blue_part = meta::quantity<float, meta::unit_t<struct blue_part_tag>>;
	using alpha_part = meta::quantity<float, meta::unit_t<struct alpha_part_tag>>;

	namespace literals {
		constexpr auto operator "" _r(long double value) { return red_part{static_cast<float>(value)}; }
		constexpr auto operator "" _g(long double value) { return green_part{static_cast<float>(value)}; }
		constexpr auto operator "" _b(long double value) { return blue_part{static_cast<float>(value)}; }
		constexpr auto operator "" _a(long double value) { return alpha_part{static_cast<float>(value)}; }
	}

	using color = vector<red_part, green_part, blue_part>;

	constexpr auto& red(color& c) { return c.get<red_part>(); }
	constexpr auto const& red(color const& c) { return c.get<red_part>(); }

	constexpr auto& green(color& c) { return c.get<green_part>(); }
	constexpr auto const& green(color const& c) { return c.get<green_part>(); }

	constexpr auto& blue(color& c) { return c.get<blue_part>(); }
	constexpr auto const& blue(color const& c) { return c.get<blue_part>(); }

	constexpr auto& alpha(color& c) { return c.get<alpha_part>(); }
	constexpr auto const& alpha(color const& c) { return c.get<alpha_part>(); }
}
