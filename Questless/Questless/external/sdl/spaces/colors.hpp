//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines color constants.

#pragma once

#include "color.hpp"

namespace units::colors {
	using namespace units::literals;

	constexpr auto clear(color_part r = 0.0_c, color_part g = 0.0_c, color_part b = 0.0_c) { return color{r, g, b, 0.0_c}; }
	constexpr auto white(color_part a = 1.0_c) { return color{1.0_c, 1.0_c, 1.0_c, a}; }
	constexpr auto silver(color_part a = 1.0_c) { return color{0.75_c, 0.75_c, 0.75_c, a}; }
	constexpr auto gray(color_part a = 1.0_c) { return color{0.5_c, 0.5_c, 0.5_c, a}; }
	constexpr auto black(color_part a = 1.0_c) { return color{0.0_c, 0.0_c, 0.0_c, a}; }
	constexpr auto red(color_part a = 1.0_c) { return color{1.0_c, 0.0_c, 0.0_c, a}; }
	constexpr auto lime(color_part a = 1.0_c) { return color{0.0_c, 1.0_c, 0.0_c, a}; }
	constexpr auto green(color_part a = 1.0_c) { return color{0.0_c, 0.75_c, 0.0_c, a}; }
	constexpr auto blue(color_part a = 1.0_c) { return color{0.0_c, 0.0_c, 1.0_c, a}; }
	constexpr auto cyan(color_part a = 1.0_c) { return color{0.0_c, 1.0_c, 1.0_c, a}; }
	constexpr auto yellow(color_part a = 1.0_c) { return color{1.0_c, 1.0_c, 0.0_c, a}; }
	constexpr auto magenta(color_part a = 1.0_c) { return color{1.0_c, 0.0_c, 1.0_c, a}; }
	constexpr auto orange(color_part a = 1.0_c) { return color{1.0_c, 0.5_c, 0.0_c, a}; }
	constexpr auto purple(color_part a = 1.0_c) { return color{0.5_c, 0.0_c, 0.5_c, a}; }
	constexpr auto teal(color_part a = 1.0_c) { return color{0.0_c, 0.5_c, 0.5_c, a}; }
	constexpr auto tan(color_part a = 1.0_c) { return color{0.75_c, 0.625_c, 0.5_c, a}; }
	constexpr auto brown(color_part a = 1.0_c) { return color{0.5_c, 0.25_c, 0.0_c, a}; }
}
