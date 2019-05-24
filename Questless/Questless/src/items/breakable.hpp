//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/misc.hpp"
#include "utility/lazy_bounded.hpp"

namespace ql {
	//! Component for items that can be worn down and broken.
	struct breakable {
		ent id;

		//! The item's durability, i.e. its maximum integrity.
		ql::integrity durability;

		//! Whether the item is broken, i.e. integrity is zero.
		bool broken() const {
			return integrity.value() <= 0.0_integrity;
		}

		//! The item's integrity, which ranges from zero to its durability. The item is broken if integrity is zero.
		lazy_bounded<ql::integrity> integrity{durability, [] { return 0.0_integrity; }, [this] { return durability; }};
	};
}
