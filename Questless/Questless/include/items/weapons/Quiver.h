/**
* @file    Quiver.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "items/Item.h"
#include "items/Inventory.h"

namespace questless
{
	/// An item for containing projectiles, such as arrows.
	////
	class Quiver : public Item
	{
	public:
		Inventory inventory;

		Quiver(Inventory inventory, Id<Item> id = Id<Item>::make())
			: inventory{std::move(inventory)}, Item{ id }
		{}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Quiver"; }

		double weight() const override { return 0.5; }

		virtual std::vector<Action::uptr> actions() override
		{
			std::vector<Action::uptr> actions;
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}
	};
}
