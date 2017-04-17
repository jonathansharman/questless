//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <stdexcept>

namespace questless
{
	class Human;
	class Goblin;

	class Corpse;
	class ItemBox;

	//! Visitor type for entities.
	class EntityVisitor
	{
	public:
		virtual ~EntityVisitor() = default;

		virtual void visit(Human const&) = 0;
		virtual void visit(Goblin const&) = 0;

		virtual void visit(Corpse const&) = 0;
		virtual void visit(ItemBox const&) = 0;
	};
}
