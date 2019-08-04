//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <entt/entity/registry.hpp>

namespace ql {
	using id = entt::registry::entity_type;

	//! @brief The global ECS registry.
	inline entt::registry reg;
}
