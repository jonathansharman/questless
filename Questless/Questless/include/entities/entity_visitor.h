//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace ql
{
	// Beings

	class goblin;
	class human;

	// Objects

	class campfire;
	class corpse;
	class item_box;

	using entity_subtype_list = type_list::of_t
		// Beings
		< goblin
		, human
		// Objects
		, campfire
		, corpse
		, item_box
		>;

	DEFINE_VISITORS(entity, entity_subtype_list)
}
