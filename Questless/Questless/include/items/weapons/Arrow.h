/**
* @file    Arrow.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

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

		std::vector<Action::uptr> actions() override
		{
			std::vector<Action::uptr> actions;
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}
	};
}
