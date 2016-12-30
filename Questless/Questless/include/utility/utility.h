/**
* @file    utility.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Interface for utility functions.
*/

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <random>
#include <cmath>
#include <functional>

#include "units/GameVector.h"
#include "units/GameRadians.h"

namespace questless
{
	///////////////
	// Debugging //
	///////////////

	double time(std::function<void()> code);

	//////////////////////////////
	// Random number generation //
	//////////////////////////////

	extern std::mt19937 rng;

	/// @return True or false with equal probability.
	inline bool random_bool()
	{
		return std::uniform_int<int>(0, 1)(rng) == 0;
	}

	/// Generates a random integral value with a uniform distribution.
	/// @param min The minimum possible value.
	/// @param max The maxium possible value.
	/// @return A random integral value in [min, max].
	template<typename Integer>
	typename std::enable_if_t<std::is_integral<Integer>::value, Integer>
	inline uniform(Integer min, Integer max)
	{
		return std::uniform_int_distribution<Integer>(min, max)(rng);
	}

	/// Generates a random floating-point value with a uniform distribution.
	/// @param min The minimum possible value.
	/// @param max The maxium possible value.
	/// @return A random floating-point value in [min, max].
	template<typename Floating>
	typename std::enable_if_t<std::is_floating_point<Floating>::value, Floating>
	inline uniform(Floating min, Floating max)
	{
		return std::uniform_real_distribution<Floating>(min, max)(rng);
	}

	/// @return a random angle in radians.
	inline units::GameRadians random_angle()
	{
		return uniform(0.0, 1.0) * units::GameRadians::circle();
	}

	/// @return A displacement based on a uniform random distance and uniform random angle.
	/// @param max_length The maximum possible length of the displacement.
	inline units::GameVector random_displacement(double max_length)
	{
		return units::GameVector{random_angle(), uniform(0.0, max_length)};
	}

	/// @return A displacement based on a uniform random distance and uniform random angle.
	/// @param min_length The minimum possible length of the displacement.
	/// @param max_length The maximum possible length of the displacement.
	inline units::GameVector random_displacement(double min_length, double max_length)
	{
		return units::GameVector{random_angle(), uniform(min_length, max_length)};
	}

	//////////
	// Math //
	//////////

	/// Converts a double in the range [0, 1] to an 8-bit integer (range [0, 255]).
	/// @param percent A double in the range [0, 1].
	/// @return The corresponding 8-bit integer.
	uint8_t percentage_to_byte(double percent);

	/// @return The square of the given value.
	template <typename T>
	constexpr T square(T value) { return value * value; }

	/// @return The cube of the given value.
	template <typename T>
	constexpr T cube(T value) { return value * value * value; }

	namespace detail
	{
		template <typename Floating, typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
		constexpr Floating constexpr_sqrt_iterative(Floating x, Floating current, Floating previous) {
			return current == previous
				? current
				: constexpr_sqrt_iterative(x, 0.5 * (current + x / current), current);
		}
	}
	/// constexpr version of sqrt function.
	/// @return The square root of the given floating-point number.
	template <typename Floating, typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
	constexpr Floating csqrt(Floating x)
	{
		/// @todo Replace detail::constexpr_sqrt_iterative with constexpr lambda when better supported.
		return detail::constexpr_sqrt_iterative(x, 0.5 * x, 0.0);
	}

	constexpr double root_three = csqrt(3.0);

	///////////////////
	// Miscellaneous //
	///////////////////

	/// Conditional element removal for unordered container types.
	/// @param container A container.
	/// @param predicate A predicate over elements of the container. Elements for which the predicate is true are removed.
	template <typename ContainerType, typename PredicateType>
	void erase_if(ContainerType& container, const PredicateType& predicate)
	{
		for (auto it = container.begin(); it != container.end();) {
			if (predicate(*it)) {
				it = container.erase(it);
			} else {
				++it;
			}
		}
	};
}

#endif
