//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	class Arrow;
	class Bow;
	class Quarterstaff;
	class Quiver;
	class Scroll;

	//! Visitor type for items.
	using ItemVisitor = Visitor
		< Arrow const
		, Bow const
		, Quarterstaff const
		, Quiver const
		, Scroll const
		>;
}
