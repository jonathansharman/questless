//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql {
	//! The in-game quantity for mass. Approximately equivalent to kg.
	using mass = cancel::quantity<double, cancel::unit_t<struct mass_tag>>;
	constexpr mass operator"" _mass(long double value) {
		return mass{static_cast<double>(value)};
	}
}
