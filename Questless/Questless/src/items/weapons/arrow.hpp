//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

namespace ql {
	struct arrow {
		reg_ptr eg;
		id id;
	};

	auto make_arrow(reg& reg, id arrow_id) -> id;
}
