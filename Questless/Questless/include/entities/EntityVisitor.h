//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	// Beings

	class Goblin;
	class Human;

	// Objects

	class Corpse;
	class ItemBox;

	using EntityMutableVisitor = Visitor
		// Beings
		< Goblin
		, Human
		// Objects
		, Corpse
		, ItemBox
		>;

	using EntityConstVisitor = Visitor
		// Beings
		< Goblin const
		, Human const
		// Objects
		, Corpse const
		, ItemBox const
		>;
}
