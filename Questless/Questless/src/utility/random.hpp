//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Convenience functions for generating various random values.

#pragma once

#include "utility.hpp"

#include "units/world_space.hpp"

#include <random>
#include <type_traits>

namespace ql {
	//! A pseudorandom number generator.
	std::mt19937_64& prng() {
		static auto result = [] {
			std::random_device seeder{};
			return std::mt19937_64{seeder};
		}();
		return result;
	}

	//! True or false with equal probability.
	inline bool random_bool() {
		return std::uniform_int<int>(0, 1)(prng()) == 0;
	}

	//! Generates a random value with a uniform distribution in [@p min, @p max].
	template<typename Arithmetic>
	typename Arithmetic uniform(Arithmetic min, Arithmetic max) {
		if constexpr (std::is_integral_v<Arithmetic>) {
			return std::uniform_int_distribution<Arithmetic>(min, max)(prng());
		} else if constexpr (std::is_floating_point_v<Arithmetic>) {
			return std::uniform_real_distribution<Arithmetic>(min, max)(prng());
		} else {
			static_assert(dependent_false_type<Arithmetic>::value, "Bounds type must be integral or floating-point.");
		}
	}

	//! True or false with @p probability chance of returning true.
	inline bool bernoulli_trial(double probability) {
		return probability > uniform(0.0, 1.0);
	}

	//! A random angle in radians.
	template <typename Space>
	typename Space::radians random_angle() {
		typename Space::radians result;
		std::fill
			( result.components.begin()
			, result.components.end()
			, uniform(Space::scalar(0), Space::scalar(1)) * units::world_space::radians::circle()
			);
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param min_length The minimum possible length of the displacement.
	//! @param max_length The maximum possible length of the displacement.
	template <typename Space>
	typename Space::vector random_displacement(typename Space::scalar min_length, typename Space::scalar max_length) {
		return units::world_space::vector{random_angle<Space>(), uniform(min_length, max_length)};
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param max_length The maximum possible length of the displacement.
	template <typename Space>
	typename Space::vector random_displacement(typename Space::scalar max_length) {
		return random_displacement(Space::scalar(0), max_length);
	}

	//! A uniformly randomly sampled point from within the bounds of @p box.
	inline units::world_space::point random_point_within(units::world_space::box box) {
		return box.position + units::world_space::vector{uniform(0.0, width(box)), uniform(0.0, height(box))};
	}
}
