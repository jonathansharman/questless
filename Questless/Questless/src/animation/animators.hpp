//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Contains functions for creating animations for various game types.

#pragma once

#include "magic/spell.hpp"
#include "utility/reference.hpp"

namespace ql {
	namespace rsrc {
		struct spell;
		struct tile;
	}
	struct animation;
	struct tile;

	//! Creates an animation using @p resources for @p tile.
	uptr<animation> animate(rsrc::tile const& resources, tile const& tile);

	//! Creates an animation using @p resources for @p spell.
	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell);
}
