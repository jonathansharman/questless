//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Item.h"

namespace questless
{
	class Arrow : public ItemBase<Arrow>
	{
	public:
		Arrow(Id<Item> id = Id<Item>::make()) : ItemBase<Arrow>{id} {}

		std::string name() const final { return "Arrow"; }

		double weight() const final { return 0.1; }

		std::vector<uptr<Action>> actions() final
		{
			std::vector<uptr<Action>> actions;
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}
	};
}
