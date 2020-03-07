//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Convenience functions for generating various random values.

#pragma once

#include "utility.hpp"

#include "world/coordinates.hpp"

#include <random>
#include <type_traits>

namespace ql {
	//! A pseudorandom number generator.
	inline auto prng = [] {
		std::random_device rng{};
		constexpr auto n = sizeof(std::mt19937_64::result_type) * std::mt19937_64::state_size / sizeof(unsigned);
		std::array<unsigned, n> seed_data;
		std::generate(seed_data.begin(), seed_data.end(), std::ref(rng));
		std::seed_seq seed(seed_data.begin(), seed_data.end());
		return std::mt19937_64{seed};
	}();

	//! True or false with equal probability.
	inline auto coin_flip() -> bool {
		return std::uniform_int<int>(0, 1)(prng) == 0;
	}

	//! Generates a random value with a uniform distribution in [@p min, @p max].
	template <typename T>
	typename auto uniform(T min, T max) -> T requires std::is_arithmetic_v<T> {
		if constexpr (std::is_integral_v<T>) {
			return std::uniform_int_distribution<T>(min, max)(prng);
		} else if constexpr (std::is_floating_point_v<T>) {
			return std::uniform_real_distribution<T>(min, max)(prng);
		}
	}

	//! Overload of the uniform sample function for quantity types.
	template <typename Rep, typename Unit>
	auto uniform(cancel::quantity<Rep, Unit> const& min, cancel::quantity<Rep, Unit> const& max) {
		return cancel::quantity<Rep, Unit>{uniform(min.value, max.value)};
	}

	//! True or false with @p probability chance of returning true.
	inline auto bernoulli_trial(double probability) -> bool {
		return probability > uniform(0.0, 1.0);
	}

	//! A uniformly randomly chosen direction in region tile space.
	inline auto random_direction() {
		return static_cast<hex_direction>(uniform(0, 5));
	}

	//! A random angle in degrees.
	inline auto random_radians() {
		return uniform(vecx::radians{0.0}, vecx::circle_rad);
	}

	//! A random angle in degrees.
	inline auto random_degrees() {
		return uniform(vecx::degrees{0.0}, vecx::circle_deg);
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param min_length The minimum possible length of the displacement.
	//! @param max_length The maximum possible length of the displacement.
	template <typename QuantityType>
	auto random_displacement(QuantityType min_length, QuantityType max_length) {
		return make_polar_vector(uniform(min_length, max_length), random_radians());
	}

	//! A displacement based on a uniform random distance and uniform random angle.
	//! @param max_length The maximum possible length of the displacement.
	template <typename QuantityType>
	auto random_displacement(QuantityType max_length) {
		return make_polar_vector(uniform(QuantityType(0), max_length), random_radians());
	}

	//! A uniformly randomly sampled point from within the bounds of @p box.
	inline auto random_point_within(view::box box) { //! @todo Check containment logic here.
		return box.position + view::vector{uniform(view::px{0.0}, width(box)), uniform(view::px{0.0}, height(box))};
	}
}
