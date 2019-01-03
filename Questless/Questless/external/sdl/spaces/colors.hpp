//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines color constants.

#pragma once

#include "vecx/vector.hpp"

#include "vecx/define_macros.hpp"

namespace sdl::spaces::colors {
	using part = cancel::quantity<float, cancel::unit_t<struct part_tag>>;

	using color = vecx::vector<part, 4>;

	namespace literals {
		//! 
		constexpr auto operator "" _c(long double value) { return part{static_cast<float>(value)}; }

		//! Creates a color from a three-digit hexadecimal number.
		constexpr auto operator "" _rgb(unsigned long long value) {
			part r = part{(value >> 16) / 255.0f};
			part g = part{((value >> 8) & 0xff) / 255.0f};
			part b = part{(value & 0xff) / 255.0f};
			return color{r, g, b, 1.0_c};
		}

		//! Creates a color from a four-digit hexadecimal number.
		constexpr auto operator "" _rgba(unsigned long long value) {
			part r = part{(value >> 24) / 255.0f};
			part g = part{(value >> 16) / 255.0f};
			part b = part{((value >> 8) & 0xff) / 255.0f};
			part a = part{(value & 0xff) / 255.0f};
			return color{r, g, b, a};
		}
	}
	using namespace literals;

	DEFINE_VECTOR_INDEX_NAME(color, 0, r);
	DEFINE_VECTOR_INDEX_NAME(color, 1, g);
	DEFINE_VECTOR_INDEX_NAME(color, 2, b);
	DEFINE_VECTOR_INDEX_NAME(color, 3, a);

	constexpr auto clear(part r = 0.0_c, part g = 0.0_c, part b = 0.0_c) { return color{r, g, b, 0.0_c}; }
	constexpr auto white(part a = 1.0_c) { return color{1.0_c, 1.0_c, 1.0_c, a}; }
	constexpr auto silver(part a = 1.0_c) { return color{0.75_c, 0.75_c, 0.75_c, a}; }
	constexpr auto gray(part a = 1.0_c) { return color{0.5_c, 0.5_c, 0.5_c, a}; }
	constexpr auto black(part a = 1.0_c) { return color{0.0_c, 0.0_c, 0.0_c, a}; }
	constexpr auto red(part a = 1.0_c) { return color{1.0_c, 0.0_c, 0.0_c, a}; }
	constexpr auto lime(part a = 1.0_c) { return color{0.0_c, 1.0_c, 0.0_c, a}; }
	constexpr auto green(part a = 1.0_c) { return color{0.0_c, 0.75_c, 0.0_c, a}; }
	constexpr auto blue(part a = 1.0_c) { return color{0.0_c, 0.0_c, 1.0_c, a}; }
	constexpr auto cyan(part a = 1.0_c) { return color{0.0_c, 1.0_c, 1.0_c, a}; }
	constexpr auto yellow(part a = 1.0_c) { return color{1.0_c, 1.0_c, 0.0_c, a}; }
	constexpr auto magenta(part a = 1.0_c) { return color{1.0_c, 0.0_c, 1.0_c, a}; }
	constexpr auto orange(part a = 1.0_c) { return color{1.0_c, 0.5_c, 0.0_c, a}; }
	constexpr auto purple(part a = 1.0_c) { return color{0.5_c, 0.0_c, 0.5_c, a}; }
	constexpr auto teal(part a = 1.0_c) { return color{0.0_c, 0.5_c, 0.5_c, a}; }
	constexpr auto tan(part a = 1.0_c) { return color{0.75_c, 0.625_c, 0.5_c, a}; }
	constexpr auto brown(part a = 1.0_c) { return color{0.5_c, 0.25_c, 0.0_c, a}; }
}

#include "vecx/undef_macros.hpp"
