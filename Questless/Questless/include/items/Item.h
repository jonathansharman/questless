/**
* @file    Item.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Item abstract base class.
*/

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "ItemVisitor.h"
#include "entities/beings/Action.h"
#include "units/HexCoords.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	class Item
	{
	public:
		using ptr = std::unique_ptr<Item>;
		using ref = std::reference_wrapper<Item>;

		Id<Item> const id;

		Item(Id<Item> id) : id{id} {}

		virtual ~Item() = default;

		virtual void accept(ItemVisitor& visitor) const = 0;

		/// @return The item's name.
		////
		virtual std::string name() const = 0;

		/// @return The item's weight.
		////
		virtual double weight() const = 0;

		/// @return The list of actions that can be performed with the item.
		////
		virtual std::vector<Action::ptr> actions() = 0;

		/// Advances the item one time unit.
		////
		virtual void update() {}
	protected:
		class Drop : public Action
		{
		public:
			Drop(Item& item) : _item{item} {}

			static ptr make(Item& item) { return std::make_unique<Drop>(item); }

			std::string name() const override { return "Drop"; }

			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};

		class Throw : public Action
		{
		public:
			Throw(Item& item) : _item{item} {}

			static ptr make(Item& item) { return std::make_unique<Throw>(item); }

			std::string name() const override { return "Throw"; }

			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};
	};
}
