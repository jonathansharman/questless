/**
* @file    constexpr-math.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Provides constexpr versions of a subset of standard math functions.
*/

#pragma once

namespace units::constexpr_math
{
	namespace detail
	{
		template <typename Floating, typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
		constexpr Floating sqrt_iterative(Floating x, Floating current, Floating previous)
		{
			return current == previous
				? current
				: sqrt_iterative(x, 0.5 * (current + x / current), current);
		}
	}

	/// constexpr version of sqrt().
	/// @return The square root of the given floating-point number.
	template <typename Floating>
	constexpr std::enable_if_t<std::is_floating_point<Floating>::value, Floating>
	sqrt(Floating x)
	{
		/// @todo Replace sqrt_iterative with constexpr lambda when better supported.
		return detail::sqrt_iterative(x, 0.5 * x, 0.0);
	}

	/// constexpr version of sqrt().
	/// @return The square root of the given integral number.
	template <typename Integer>
	constexpr std::enable_if_t<std::is_integral<Integer>::value, Integer>
	sqrt(Integer x)
	{
		/// @todo Replace sqrt_iterative with constexpr lambda when better supported.
		return static_cast<Integer>(detail::sqrt_iterative(static_cast<double>(x), 0.5 * x, 0.0));
	}
}
