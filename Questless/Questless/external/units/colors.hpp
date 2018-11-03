//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines color constants.

#pragma once

#include "color.hpp"

namespace units::colors {
	using namespace units::literals;

	constexpr auto clear(red_part r = 0.0_r, green_part g = 0.0_g, blue_part b = 0.0_b) { return color{r, g, b, 0.0_a}; }
	constexpr auto white(alpha_part a = 1.0_a) { return color{1.0_r, 1.0_g, 1.0_b, a}; }
	constexpr auto silver(alpha_part a = 1.0_a) { return color{0.75_r, 0.75_g, 0.75_b, a}; }
	constexpr auto gray(alpha_part a = 1.0_a) { return color{0.5_r, 0.5_g, 0.5_b, a}; }
	constexpr auto black(alpha_part a = 1.0_a) { return color{0.0_r, 0.0_g, 0.0_b, a}; }
	constexpr auto red(alpha_part a = 1.0_a) { return color{1.0_r, 0.0_g, 0.0_b, a}; }
	constexpr auto lime(alpha_part a = 1.0_a) { return color{0.0_r, 1.0_g, 0.0_b, a}; }
	constexpr auto green(alpha_part a = 1.0_a) { return color{0.0_r, 0.75_g, 0.0_b, a}; }
	constexpr auto blue(alpha_part a = 1.0_a) { return color{0.0_r, 0.0_g, 1.0_b, a}; }
	constexpr auto cyan(alpha_part a = 1.0_a) { return color{0.0_r, 1.0_g, 1.0_b, a}; }
	constexpr auto yellow(alpha_part a = 1.0_a) { return color{1.0_r, 1.0_g, 0.0_b, a}; }
	constexpr auto magenta(alpha_part a = 1.0_a) { return color{1.0_r, 0.0_g, 1.0_b, a}; }
	constexpr auto orange(alpha_part a = 1.0_a) { return color{1.0_r, 0.5_g, 0.0_b, a}; }
	constexpr auto purple(alpha_part a = 1.0_a) { return color{0.5_r, 0.0_g, 0.5_b, a}; }
	constexpr auto teal(alpha_part a = 1.0_a) { return color{0.0_r, 0.5_g, 0.5_b, a}; }
	constexpr auto tan(alpha_part a = 1.0_a) { return color{0.75_r, 0.625_g, 0.5_b, a}; }
	constexpr auto brown(alpha_part a = 1.0_a) { return color{0.5_r, 0.25_g, 0.0_b, a}; }
}
