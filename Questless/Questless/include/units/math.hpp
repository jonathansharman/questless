//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Provides common math functions, including constexpr versions of a subset of standard math functions.

#pragma once

#include <type_traits>

namespace units::math {
	namespace detail {
		//! @todo Replace sqrt_iterative with constexpr lambda if recursive lambdas are added. Also, reimplement as a loop once supported.
		template <typename Floating>
		constexpr Floating sqrt_iterative(Floating const& x, Floating const& current, Floating const& previous) {
			return current == previous
				? current
				: sqrt_iterative(x, static_cast<Floating>(0.5 * (current + x / current)), current);
		}

		template <typename T>
		struct dependent_false_type : std::false_type {};
	}

	//! The square root of the integral number @p x.
	template <typename Arithmetic>
	constexpr Arithmetic sqrt(Arithmetic x) {
		if constexpr (std::is_integral_v<Arithmetic>) {
			return static_cast<Arithmetic>(sqrt(static_cast<double>(x)));
		} else if constexpr (std::is_floating_point_v<Arithmetic>) {
			return detail::sqrt_iterative(x, static_cast<Arithmetic>(0.5 * x), static_cast<Arithmetic>(0.0));
		} else {
			static_assert(detail::dependent_false_type<Arithmetic>::value, "sqrt() requires an integral or floating-point argument.");
		}
	}

	//! The square of @p value.
	template <typename T>
	constexpr T square(T const& value) { return value * value; }

	//! The cube of the @p value.
	template <typename T>
	constexpr T cube(T const& value) { return value * value * value; }

	//! The absolute value of @p value.
	template <typename T>
	constexpr T abs(T const& value) {
		return value < static_cast<T>(0) ? -value : value;
	}

	template <typename Floating, typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
	constexpr long lround(Floating const& x) {
		return x > 0 ? static_cast<long>(x + 0.5) : static_cast<long>(x - 0.5);
	}
}
