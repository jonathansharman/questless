//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Contains functions for creating animations for various game types.

#pragma once

#include "utility/reference.hpp"

namespace ql {
	namespace magic {
		struct spell;
	}
	namespace rsrc {
		struct entity;
		struct spell;
		struct tile;
	}
	struct animation;
	struct being;
	struct entity;
	struct object;
	struct tile;

	//! Creates an animation using @p resources for @p tile.
	uptr<animation> animate(rsrc::tile const& resources, tile const& tile);

	//! Creates an animation using @p resources for @p spell.
	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell);

	//! Creates an animation using @p resources for @p entity.
	uptr<animation> animate(rsrc::entity const& resources, entity const& entity);

	//! Creates an animation using @p resources for @p being.
	uptr<animation> animate(rsrc::entity const& resources, being const& being);

	//! Creates an animation using @p resources for @p object.
	uptr<animation> animate(rsrc::entity const& resources, object const& object);
}
