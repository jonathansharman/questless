//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Convenience functions for generating various random values.

#pragma once

#include "utility.hpp"

#include "world/coordinates.hpp"

#include <random>
#include <type_traits>

namespace ql {
	//! A pseudorandom number generator.
	std::mt19937_64& prng() {
		static auto result = [] {
			std::random_device seeder{};
			return std::mt19937_64{seeder()};
		}();
		return result;
	}

	//! True or false with equal probability.
	inline bool random_bool() {
		return std::uniform_int<int>(0, 1)(prng()) == 0;
	}

	//! Generates a random value with a uniform distribution in [@p min, @p max].
	template <typename Arithmetic>
	typename Arithmetic uniform(Arithmetic min, Arithmetic max) {
		if constexpr (std::is_integral_v<Arithmetic>) {
			return std::uniform_int_distribution<Arithmetic>(min, max)(prng());
		} else if constexpr (std::is_floating_point_v<Arithmetic>) {
			return std::uniform_real_distribution<Arithmetic>(min, max)(prng());
		} else {
			static_assert(dependent_false_type<Arithmetic>::value, "Bounds type must be integral or floating-point.");
		}
	}

	//! Overload of the uniform sample function for quantity types.
	template <typename Rep, typename Unit>
	auto uniform(cancel::quantity<Rep, Unit> const& min, cancel::quantity<Rep, Unit> const& max) {
		return cancel::quantity<Rep, Unit>{uniform(min.value, max.value)};
	}

	//! True or false with @p probability chance of returning true.
	inline bool bernoulli_trial(double probability) {
		return probability > uniform(0.0, 1.0);
	}

	//! A uniformly randomly chosen direction in region tile space.
	inline region_tile::direction random_direction() {
		return static_cast<region_tile::direction>(uniform(1, 6));
	}

	//! A random angle in degrees.
	typename vecx::radians random_radians() {
		return uniform(vecx::radians{0.0}, vecx::circle_rad);
	}

	//! A random angle in degrees.
	typename vecx::degrees random_degrees() {
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
	inline view::point random_point_within(view::box box) { //! @todo Check containment logic here.
		return box.position + view::vector{uniform(view::px{0.0}, width(box)), uniform(view::px{0.0}, height(box))};
	}
}
