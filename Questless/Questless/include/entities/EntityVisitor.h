//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace questless
{
	// Beings

	class Goblin;
	class Human;

	// Objects

	class Corpse;
	class ItemBox;

	using EntitySubtypeList = type_list::of_t
		// Beings
		< Goblin
		, Human
		// Objects
		, Corpse
		, ItemBox
		>;

	DEFINE_VISITORS(Entity, EntitySubtypeList)
}
