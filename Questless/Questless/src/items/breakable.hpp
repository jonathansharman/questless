//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/dynamic_nonnegative.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

namespace ql {
	//! Component for items that can be worn down and broken.
	struct breakable {
		ent id;

		//! The item's current and maximum durability. The item is broken if durability is zero.
		dynamic_nonnegative<ql::durability> durability;

		//! Whether the item is broken, i.e. integrity is zero.
		bool broken() const {
			return durability.value() <= 0_durability;
		}
	};

	void make_breakable(ent id, dynamic_nonnegative<ql::durability> durability);
}
