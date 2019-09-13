//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "being.hpp"

#include "entities/entity.hpp"
#include "items/inventory.hpp"

namespace ql {
	auto make_being(id being_id, location location, agent agent, body body) -> id {
		make_entity(being_id, location);
		reg.assign<ql::agent>(being_id, std::move(agent));
		reg.assign<ql::body>(being_id, std::move(body));
		reg.assign<inventory>(being_id);

		return being_id;
	}
}
