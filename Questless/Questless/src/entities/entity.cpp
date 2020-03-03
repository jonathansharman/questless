//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "entity.hpp"

namespace ql {
	//! Makes @p entity_id an entity: a being or object that can exist in the world.
	auto make_entity(reg& reg, id entity_id, location location) -> id {
		reg.assign<ql::location>(entity_id, location);
		return entity_id;
	}
}
