/**
* @file    ItemVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the Item class.
*/

#pragma once

#include <string>
#include <stdexcept>

namespace questless
{
	class Arrow;
	class Bow;
	class Quarterstaff;
	class Quiver;
	class Scroll;

	class ItemVisitor
	{
	public:
		virtual ~ItemVisitor() = default;

		virtual void visit(Arrow const&) = 0;
		virtual void visit(Bow const&) = 0;
		virtual void visit(Quarterstaff const&) = 0;
		virtual void visit(Quiver const&) = 0;
		virtual void visit(Scroll const&) = 0;
	};
}
