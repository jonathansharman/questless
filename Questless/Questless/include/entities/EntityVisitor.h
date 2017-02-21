/**
* @file    EntityVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Entity visitor abstract base class.
*/

#pragma once

#include <string>
#include <stdexcept>

namespace questless
{
	class Human;
	class Goblin;

	class Corpse;
	class ItemBox;

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
