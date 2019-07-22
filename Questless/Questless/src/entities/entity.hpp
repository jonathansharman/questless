//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "transparency.hpp"

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! Makes @p id an entity: a being or object that can exist in the world.
	void make_entity(ent id, location location, transparency transparency);
}
