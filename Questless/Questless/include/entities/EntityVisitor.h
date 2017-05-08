//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	// Beings

	class Goblin;
	class Human;

	// Objects

	class Campfire;
	class Corpse;
	class ItemBox;

	using EntitySubtypeList = type_list::of_t
		// Beings
		< Goblin
		, Human
		// Objects
		, Campfire
		, Corpse
		, ItemBox
		>;

	DEFINE_VISITORS(Entity, EntitySubtypeList)
}
