//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <memory>

#include "item.hpp"

namespace ql
{
	//! An item that can be crafted from other items.
	class craftable : public virtual item
	{
	public:
		virtual ~craftable() = 0 {}

		enum class action { mix, boil, smoke, crush };
	protected:
		action _action;
		//! @todo Ingredients?
	};
}
