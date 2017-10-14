//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Miscellaneous basic utility functions for math, RNG, debugging, etc.

#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <fstream>
#include <random>
#include <string>
#include <sstream>

#include "units/game_space.h"

namespace ql
{
	///////////////
	// Debugging //
	///////////////

	//! Executes and times the function @p f.
	//! @return The time it took to execute @p f, in nanoseconds.
	std::chrono::nanoseconds time(std::function<void()> f);

	//////////////
	// File I/O //
	//////////////

	//! The contents, as a string, of the file with filename @p filename.
	std::string contents_of_file(char const* filename);

	//////////
	// Math //
	//////////

	//! Converts @p percent in the range [0, 1] to an 8-bit integer in the range [0, 255].
	uint8_t percentage_to_byte(double percent);

	///////////////////
	// Miscellaneous //
	///////////////////

	//! Conditionally removes elements from a container.
	//! @param container An iterable container.
	//! @param predicate A predicate over elements of the container. Elements for which the predicate is true are removed.
	template <typename ContainerType, typename PredicateType>
	void erase_if(ContainerType& container, PredicateType&& predicate)
	{
		for (auto it = container.begin(); it != container.end();) {
			if (std::forward<PredicateType>(predicate)(*it)) {
				it = container.erase(it);
			} else {
				++it;
			}
		}
	};

	//! @todo Why do the following lambda overload helpers not work?

	/*template <typename... Functors>
	struct overloaded : Functors...
	{
		using Functors::operator ()...;
	};

	template <typename... Functors>
	overloaded<Functors...> overload(Functors...)
	{
		return overloaded<Functors...>{};
	}*/

	//////////////////////////////
	// Random Number Generation //
	//////////////////////////////

	extern std::mt19937_64 rng;

	//! True or false with equal probability.
	inline bool random_bool()
	{
		return std::uniform_int<int>(0, 1)(rng) == 0;
	}

	//! Generates a random integral value with a uniform distribution in [@p min, @p max].
	template<typename Integer>
	typename std::enable_if_t<std::is_integral<Integer>::value, Integer>
		uniform(Integer min, Integer max)
	{
		return std::uniform_int_distribution<Integer>(min, max)(rng);
	}

	//! Generates a random floating-point value with a uniform distribution in [@p min, @p max].
	template<typename Floating>
	typename std::enable_if_t<std::is_floating_point<Floating>::value, Floating>
		uniform(Floating min, Floating max)
	{
		return std::uniform_real_distribution<Floating>(min, max)(rng);
	}

	//! A random angle in radians.
	inline units::game_space::radians random_angle()
	{
		return uniform(0.0, 1.0) * units::game_space::radians::circle();
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param max_length The maximum possible length of the displacement.
	inline units::game_space::vector random_displacement(double max_length)
	{
		return units::game_space::vector{random_angle(), uniform(0.0, max_length)};
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param min_length The minimum possible length of the displacement.
	//! @param max_length The maximum possible length of the displacement.
	inline units::game_space::vector random_displacement(double min_length, double max_length)
	{
		return units::game_space::vector{random_angle(), uniform(min_length, max_length)};
	}
}
