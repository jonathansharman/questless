//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <memory>

#include "Item.h"

namespace questless
{
	//! An item that can be crafted from other items.
	class Craftable : public virtual Item
	{
	public:
		virtual ~Craftable() = 0 {}

		enum class Action {mix, boil, smoke, crush};
	protected:
		Action _action;
		//! @todo Ingredients?
	};
}
