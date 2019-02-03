//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	// Beings

	struct goblin;
	struct human;

	// Objects

	struct campfire;
	struct corpse;
	struct item_box;

	using entity_subtype_list = cancel::list_t
		// Beings
		< goblin
		, human
		// Objects
		, campfire
		, corpse
		, item_box
		>;

	DEFINE_VISITORS(entity, entity_subtype_list)

	enum class entity_subtype : unsigned
		// Beings
		{ human_class
		, goblin_class
		, troll_class
		// Objects
		, campfire
		, corpse_class
		, item_box_class
		};
}
