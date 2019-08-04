//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "breakable.hpp"

namespace ql {
	void make_breakable(id id, dynamic_nonnegative<ql::durability> durability) {
		reg.assign<breakable>(id, id, durability);
	}
}
