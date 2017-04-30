//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace questless
{
	class Arrow;
	class Bow;
	class Quarterstaff;
	class Quiver;
	class Scroll;

	using ItemSubtypeList = type_list::of_t
		< Arrow
		, Bow
		, Quarterstaff
		, Quiver
		, Scroll
		>;

	DEFINE_VISITORS(Item, ItemSubtypeList)
}
