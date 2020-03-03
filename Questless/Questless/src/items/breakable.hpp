//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded/dynamic_nonnegative.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

namespace ql {
	//! Component for items that can be worn down and broken.
	struct breakable {
		id id;

		//! The item's current and maximum durability. The item is broken if durability is zero.
		dynamic_nonnegative<ql::durability> durability;

		//! Whether the item is broken, i.e. integrity is zero.
		auto broken() const -> bool {
			return durability.value() <= 0_durability;
		}
	};

	auto make_breakable(reg& reg, id breakable_id, dynamic_nonnegative<ql::durability> durability) -> id;
}
