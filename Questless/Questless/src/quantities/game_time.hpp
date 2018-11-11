//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

#include <chrono>

namespace ql {
	//! In-game time duration.
	using tick = cancel::quantity<int, cancel::unit_t<struct tick_tag>>;
	constexpr tick operator "" _tick(unsigned long long value) { return tick{static_cast<int>(value)}; }

	//! In-game rate.
	using per_tick = cancel::inverse_t<tick>;
	constexpr per_tick operator "" _per_tick(unsigned long long value) { return per_tick{static_cast<int>(value)}; }
}
