//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

#include "items/item.hpp"
#include "items/craftable.hpp"

namespace ql {
	//! A drinkable magic concoction.
	class potion : public item, public craftable {
	public:
		virtual ~potion() = default;
	};
}
