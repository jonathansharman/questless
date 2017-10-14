//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

#include "items/item.h"
#include "items/craftable.h"

namespace ql
{
	//! A drinkable magic concoction.
	class potion : public item, public craftable
	{
	public:
		virtual ~potion() = 0 {}
	protected:
		std::function<void> _effect;
	};
}
