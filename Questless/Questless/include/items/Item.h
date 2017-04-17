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

namespace questless
{
	class Being;

	//! An item that can be picked up, used by beings, etc.
	class Item
	{
	public:
		using uptr = std::unique_ptr<Item>;
		using ref = std::reference_wrapper<Item>;
		using cref = std::reference_wrapper<Item const>;

		Id<Item> const id;

		Item(Id<Item> id) : id{id} {}

		virtual ~Item() = default;

		virtual void accept(ItemVisitor& visitor) const = 0;

		//! The item's name.
		virtual std::string name() const = 0;

		//! The item's weight.
		virtual double weight() const = 0;

		//! The list of actions that can be performed with the item.
		virtual std::vector<Action::uptr> actions() = 0;

		//! Advances the item one time unit.
		virtual void update() {}
	protected:
		class Drop : public Action
		{
		public:
			Drop(Item& item) : _item{item} {}

			static uptr make(Item& item) { return std::make_unique<Drop>(item); }

			std::string name() const override { return "Drop"; }

			Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};

		class Throw : public Action
		{
		public:
			Throw(Item& item) : _item{item} {}

			static uptr make(Item& item) { return std::make_unique<Throw>(item); }

			std::string name() const override { return "Throw"; }

			Complete perform(Being& actor, cont_t cont) override;
		private:
			Item& _item;
		};
	};
}
