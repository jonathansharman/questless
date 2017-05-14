//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Provides common math functions, including constexpr versions of a subset of standard math functions.

#pragma once

namespace units::math
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

	//! The square root of the floating-point number @p x.
	template <typename Floating>
	constexpr std::enable_if_t<std::is_floating_point<Floating>::value, Floating>
	sqrt(Floating x)
	{
		//! @todo Replace sqrt_iterative with constexpr lambda when better supported.
		return detail::sqrt_iterative(x, 0.5 * x, 0.0);
	}

	//! The square root of the integral number @p x.
	template <typename Integer>
	constexpr std::enable_if_t<std::is_integral<Integer>::value, Integer>
	sqrt(Integer x)
	{
		//! @todo Replace sqrt_iterative with constexpr lambda when better supported.
		return static_cast<Integer>(detail::sqrt_iterative(static_cast<double>(x), 0.5 * x, 0.0));
	}

	//! The square of @p value.
	template <typename T>
	constexpr T square(T value) { return value * value; }

	//! The cube of the @p value.
	template <typename T>
	constexpr T cube(T value) { return value * value * value; }

	template <typename T>
	constexpr T abs(T x)
	{
		return x < T(0) ? -x : x;
	}

	template <typename Floating, typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
	constexpr long lround(Floating x)
	{
		return x > 0 ? static_cast<long>(x + 0.5) : static_cast<long>(x - 0.5);
	}
}
