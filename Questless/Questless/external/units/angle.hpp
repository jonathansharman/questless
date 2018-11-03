//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "constants.hpp"

#include "meta/quantity.hpp"

namespace units {
	using radians = meta::quantity<double, meta::unit_t<struct radians_tag>>;

	using degrees = meta::quantity<double, meta::unit_t<struct degrees_tag>>;

	using gradians = meta::quantity<double, meta::unit_t<struct gradians_tag>>;

	namespace literals {
		auto operator "" _rad(long double value) { return radians{static_cast<double>(value)}; }
		auto operator "" _deg(long double value) { return degrees{static_cast<double>(value)}; }
		auto operator "" _grad(long double value) { return gradians{static_cast<double>(value)}; }
	}

	constexpr auto circle_rad = radians{constants::tau};
	constexpr auto circle_deg = degrees{360.0};
	constexpr auto circle_grad = gradians{400.0};

	constexpr auto rad_per_deg = circle_rad / circle_deg;
	constexpr auto rad_per_grad = circle_rad / circle_grad;

	constexpr auto deg_per_rad = circle_deg / circle_rad;
	constexpr auto deg_per_grad = circle_deg / circle_grad;

	constexpr auto grad_per_rad = circle_grad / circle_rad;
	constexpr auto grad_per_deg = circle_grad / circle_deg;
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[angle] operations") {
	SUBCASE("conversions") {
		using namespace units;

		CHECK(circle_rad * deg_per_rad == circle_deg);
		CHECK(circle_rad * grad_per_rad == circle_grad);

		CHECK(circle_deg * rad_per_deg == circle_rad);
		CHECK(circle_deg * grad_per_deg == circle_grad);

		CHECK(circle_grad * rad_per_grad == circle_rad);
		CHECK(circle_grad * deg_per_grad == circle_deg);
	}
}
