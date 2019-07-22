//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "breakable.hpp"

namespace ql {
	void make_breakable(ent id, ql::integrity integrity, ql::integrity durability) {
		reg.assign<breakable>(id, id, integrity, durability);
	}
}
