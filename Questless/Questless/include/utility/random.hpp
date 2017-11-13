//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Convenience functions for generating various random values.

#pragma once

#include <random>

#include "units/game_space.hpp"

namespace ql
{
	extern std::mt19937_64 rng;

	//! True or false with equal probability.
	inline bool random_bool()
	{
		return std::uniform_int<int>(0, 1)(rng) == 0;
	}

	//! Generates a random integral value with a uniform distribution in [@p min, @p max].
	template<typename Integer>
	typename std::enable_if_t<std::is_integral_v<Integer>, Integer>
		uniform(Integer min, Integer max)
	{
		return std::uniform_int_distribution<Integer>(min, max)(rng);
	}

	//! Generates a random floating-point value with a uniform distribution in [@p min, @p max].
	template<typename Floating>
	typename std::enable_if_t<std::is_floating_point_v<Floating>, Floating>
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
