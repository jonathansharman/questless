//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql::dmg {
	using coverage = cancel::quantity<double, cancel::unit_t<struct coverage_tag>>;

	constexpr dmg::coverage operator"" _coverage(long double value) {
		return dmg::coverage{static_cast<double>(value)};
	}

	constexpr coverage min_coverage = 0.0_coverage;
	constexpr coverage max_coverage = 100.0_coverage;
}
