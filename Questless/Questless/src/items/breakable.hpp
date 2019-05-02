//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"
#include "quantities/misc.hpp"
#include "utility/lazy_bounded.hpp"

namespace ql {
	//! Abstract base class for items which can be worn down and broken.
	struct breakable : virtual item {
		virtual ~breakable() = default;

		//! The item's durability, i.e. its maximum integrity.
		virtual ql::integrity durability() const = 0;

		//! Whether the item is broken, i.e. integrity is zero.
		bool broken() const {
			return integrity.value() <= 0.0_integrity;
		}

		//! The item's integrity, which ranges from zero to its durability. The item is broken if integrity is zero.
		lazy_bounded<ql::integrity> integrity;

	protected:
		breakable(ql::integrity integrity)
			: integrity{integrity, [] { return 0.0_integrity; }, [this] { return durability(); }} {}
	};

	DEFINE_ELEMENT_BASE_MAKE_CTOR(breakable, item)
}
