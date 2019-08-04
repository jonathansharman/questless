//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "campfire.hpp"

#include "entities/entity.hpp"
#include "world/light_source.hpp"

namespace ql {
	id make_campfire(id id, location location) {
		make_entity(id, location);
		reg.assign<light_source>(id, id, 100_lum);
		return id;
	}
}
