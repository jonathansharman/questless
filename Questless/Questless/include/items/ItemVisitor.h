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
	class Scroll;
	class Quarterstaff;

	class ItemVisitor
	{
	public:
		virtual ~ItemVisitor() = default;

		virtual void visit(Scroll const&) = 0;
		virtual void visit(Quarterstaff const&) = 0;
	};
}
