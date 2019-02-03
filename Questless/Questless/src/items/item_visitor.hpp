//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	struct arrow;
	struct bow;
	struct gatestone;
	struct quarterstaff;
	struct quiver;
	struct scroll;

	using item_subtype_list = cancel::list_t
		< arrow
		, bow
		, gatestone
		, quarterstaff
		, quiver
		, scroll
		>;

	DEFINE_VISITORS(item, item_subtype_list)
}
