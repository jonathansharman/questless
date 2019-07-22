//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "being.hpp"

#include "entities/beings/status_set.hpp"
#include "entities/entity.hpp"
#include "items/inventory.hpp"

namespace ql {
	void make_being(ent id, location location, agent agent, body body, transparency transparency) {
		make_entity(id, location, transparency);
		reg.assign<ql::agent>(id, std::move(agent));
		reg.assign<ql::body>(id, std::move(body));
		reg.assign<inventory>(id);
	}
}
