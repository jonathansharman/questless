//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql {
	//! In-game mass.
	using mass = cancel::quantity<double, cancel::unit_t<struct load_tag>>;
	constexpr mass operator"" _mass(long double value) {
		return mass{static_cast<double>(value)};
	}
}
