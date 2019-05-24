//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "campfire.hpp"

#include "entities/entity.hpp"
#include "world/light_source.hpp"

namespace ql {
	void make_campfire(ent id, location location) {
		make_entity(id, location, transparency{1.0}, true);
		reg.assign<light_source>(id, id, 100.0_lum);
	}
}
