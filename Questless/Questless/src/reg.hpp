//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <entt/entity/registry.hpp>

#include <gsl/pointers>

#include <functional>

namespace ql {
	//! ID of an entt entity.
	using id = entt::registry::entity_type;

	//! The entt registry type used to manage Questless entities.
	using reg = entt::registry;

	//! Non-null pointer to a registry.
	using reg_ptr = gsl::not_null<reg*>;
}
