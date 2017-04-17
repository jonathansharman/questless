//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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

	//! Visitor type for items.
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
