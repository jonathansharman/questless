//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! A simple source of heat and light.
	struct campfire {};

	auto make_campfire(reg& reg, id id, location location) -> ql::id;
}
