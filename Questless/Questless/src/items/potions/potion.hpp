//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

#include "items/craftable.hpp"
#include "items/item.hpp"

namespace ql {
	//! A drinkable magic concoction.
	struct potion
		: item
		, craftable {
		virtual ~potion() = default;
	};
}
