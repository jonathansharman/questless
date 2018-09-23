//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	class arrow;
	class bow;
	class gatestone;
	class quarterstaff;
	class quiver;
	class scroll;

	using item_subtype_list = meta::list_t
		< arrow
		, bow
		, gatestone
		, quarterstaff
		, quiver
		, scroll
		>;

	DEFINE_VISITORS(item, item_subtype_list)
}
