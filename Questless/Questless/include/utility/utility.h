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
#include <chrono>
#include <cmath>
#include <functional>

#include "constants.h"

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
	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
	inline uniform(T min, T max)
	{
		return std::uniform_int_distribution<T>(min, max)(rng);
	}

	/// Generates a random floating-point value with a uniform distribution.
	/// @param min The minimum possible value.
	/// @param max The maxium possible value.
	/// @return A random floating-point value in [min, max].
	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value, T>::type
	inline uniform(T min, T max)
	{
		return std::uniform_real_distribution<T>(min, max)(rng);
	}

	/// @return A displacement based on a uniform random distance and uniform random angle.
	/// @param max_length The maximum possible length of the displacement.
	VectorF random_displacement(double max_length);

	/// @return A displacement based on a uniform random distance and uniform random angle.
	/// @param min_length The minimum possible length of the displacement.
	/// @param max_length The maximum possible length of the displacement.
	VectorF random_displacement(double min_length, double max_length);

	//////////
	// Math //
	//////////

	/// Converts a double in the range [0, 1] to an 8-bit integer (range [0, 255]).
	/// @param percent A double in the range [0, 1].
	/// @return The corresponding 8-bit integer.
	uint8_t percentage_to_byte(double percent);

	/// Extends the given bounding rectangle by the given point.
	void extend_bounds(sdl::Rect& bounds, sdl::Point point);

	/// @return The square of the given value.
	template <typename T>
	T square(T value) { return value * value; }

	/// @return The cube of the given value.
	template <typename T>
	T cube(T value) { return value * value * value; }

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
