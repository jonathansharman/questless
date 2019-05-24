//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! A simple source of heat and light.
	struct campfire {};

	void make_campfire(ent id, location location);
}
