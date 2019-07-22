//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"

#include "cancel/quantity.hpp"

#include <variant>

namespace ql {
	using laceration = cancel::quantity<int, cancel::unit_t<struct laceration_tag>>;
	constexpr auto operator"" _laceration(unsigned long long value) {
		return laceration{static_cast<int>(value)};
	}

	using puncture = cancel::quantity<int, cancel::unit_t<struct puncture_tag>>;
	constexpr auto operator"" _puncture(unsigned long long value) {
		return puncture{static_cast<int>(value)};
	}

	using bruise = cancel::quantity<int, cancel::unit_t<struct bruise_tag>>;
	constexpr auto operator"" _bruise(unsigned long long value) {
		return bruise{static_cast<int>(value)};
	}

	using fracture = cancel::quantity<int, cancel::unit_t<struct fracture_tag>>;
	constexpr auto operator"" _fracture(unsigned long long value) {
		return fracture{static_cast<int>(value)};
	}

	using burn = cancel::quantity<int, cancel::unit_t<struct burns_tag>>;
	constexpr auto operator"" _burn(unsigned long long value) {
		return burn{static_cast<int>(value)};
	}

	using frostbite = cancel::quantity<int, cancel::unit_t<struct frostbite_tag>>;
	constexpr auto operator"" _frostbite(unsigned long long value) {
		return frostbite{static_cast<int>(value)};
	}

	using wound = std::variant<laceration, puncture, bruise, fracture, burn, frostbite>;
}
