//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "being.hpp"

#include "entities/entity.hpp"
#include "items/inventory.hpp"

namespace ql {
	id make_being(id id, location location, agent agent, body body) {
		make_entity(id, location);
		reg.assign<ql::agent>(id, std::move(agent));
		reg.assign<ql::body>(id, std::move(body));
		reg.assign<inventory>(id);
		return id;
	}
}
