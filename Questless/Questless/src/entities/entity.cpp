//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entity.hpp"

namespace ql {
	//! Makes @p id an entity: a being or object that can exist in the world.
	ent make_entity(ent id, location location) {
		reg.assign<ql::location>(id, location);
		return id;
	}
}
