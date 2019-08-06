//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Contains functions for creating animations for various game types.

#pragma once

#include "reg.hpp"
#include "utility/reference.hpp"

namespace ql {
	namespace magic {
		struct spell;
	}
	namespace rsrc {
		struct item;
		struct spell;
	}
	struct animation;

	//! Creates an animation for @p spell.
	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell);

	//! Creates an animation for @p item.
	uptr<animation> animate_item(rsrc::item const& item_resources, rsrc::spell const& spell_resources, id item_id);
}
