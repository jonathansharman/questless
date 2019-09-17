//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

namespace ql {
	struct arrow {
		id id;
	};

	auto make_arrow(id arrow_id) -> id;
}
