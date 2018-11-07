//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/vector.hpp"

namespace units {
	using color_part = meta::quantity<float, meta::unit_t<struct color_part_tag>>;

	namespace literals {
		constexpr auto operator "" _c(long double value) { return color_part{static_cast<float>(value)}; }
	}

	using color = vector<color_part, 4>;

	constexpr auto& red(color& c) { return c[0]; }
	constexpr auto const& red(color const& c) { return c[0]; }

	constexpr auto& green(color& c) { return c[1]; }
	constexpr auto const& green(color const& c) { return c[1]; }

	constexpr auto& blue(color& c) { return c[2]; }
	constexpr auto const& blue(color const& c) { return c[2]; }

	constexpr auto& alpha(color& c) { return c[3]; }
	constexpr auto const& alpha(color const& c) { return c[3]; }
}
