//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "corpse.hpp"

#include "entities/entity.hpp"
#include "entities/transparency.hpp"

namespace ql {
	void make_corpse(ent id, ent being_id) {
		make_entity(id, reg.get<location>(being_id), transparency{0.5}, true);
		reg.assign<corpse>(id, being_id);
	}
}
