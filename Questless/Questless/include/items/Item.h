//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "ItemVisitor.h"
#include "agents/Action.h"
#include "units/HexCoords.h"
#include "utility/Id.h"
#include "utility/reference.h"

namespace questless
{
	class Being;

	//! An item that can be picked up, used by beings, etc.
	class Item : public Element<ItemMutableVisitor, ItemConstVisitor>
	{
	public:
		Id<Item> const id;

		virtual ~Item() = default;

		//! The item's name.
		virtual std::string name() const = 0;

		//! The item's weight.
		virtual double weight() const = 0;

		//! The list of actions that can be performed with the item.
		virtual std::vector<uptr<Action>> actions() = 0;

		//! Advances the item one time unit.
		virtual void update() {}
	protected:
		class Drop : public Action
		{
		public:
			Drop(Item& item) : _item{item} {}

			static auto make(Item& item) { return std::make_unique<Drop>(item); }

			std::string name() const override { return "Drop"; }

			Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};

		class Throw : public Action
		{
		public:
			Throw(Item& item) : _item{item} {}

			static auto make(Item& item) { return std::make_unique<Throw>(item); }

			std::string name() const override { return "Throw"; }

			Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};

		Item(Id<Item> id) : id{id} {}
	};

	DEFINE_ELEMENT_BASE(Item, Item)
}
