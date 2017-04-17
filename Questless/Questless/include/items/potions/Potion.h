//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

#include "items/Item.h"
#include "items/Craftable.h"

namespace questless
{
	//! A drinkable magic concoction.
	class Potion : public Item, public Craftable
	{
	public:
		virtual ~Potion() = 0 {}
	protected:
		std::function<void> _effect;
	};
}
