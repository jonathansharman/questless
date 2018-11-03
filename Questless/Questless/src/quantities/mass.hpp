//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "meta/quantity.hpp"

namespace ql {
	//! In-game mass.
	using load = meta::quantity<double, meta::unit_t<struct load_tag>>;
	constexpr load operator "" _load(long double value) { return load{static_cast<double>(value)}; }
}
