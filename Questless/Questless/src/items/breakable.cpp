//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "breakable.hpp"

namespace ql {
	auto make_breakable(id breakable_id, dynamic_nonnegative<ql::durability> durability) -> id {
		reg.assign<breakable>(breakable_id, breakable_id, durability);

		return breakable_id;
	}
}
