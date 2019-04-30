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
		struct particle;
		struct spell;
		struct tile;
	}
	struct animation;
	struct being;
	struct entity;
	struct object;
	struct tile;

	//! Creates an animation for @p tile.
	uptr<animation> animate(rsrc::tile const& resources, tile const& tile);

	//! Creates an animation for @p spell.
	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell);

	//! Creates an animation for @p entity.
	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, entity const& entity);

	//! Creates an animation for @p being.
	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, being const& being);

	//! Creates an animation for @p object.
	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, object const& object);
}
