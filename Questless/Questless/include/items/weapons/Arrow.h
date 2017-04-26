//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Item.h"

namespace questless
{
	class Arrow : public Item
	{
	public:
		Arrow(Id<Item> id = Id<Item>::make()) : Item{id} {}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Arrow"; }

		double weight() const override { return 0.1; }

		std::vector<uptr<Action>> actions() override
		{
			std::vector<uptr<Action>> actions;
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}
	};
}
