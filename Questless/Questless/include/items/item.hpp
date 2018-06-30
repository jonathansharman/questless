//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "item_visitor.hpp"
#include "agents/action.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

namespace ql
{
	class being;

	//! An item that can be picked up, used by beings, etc.
	class item : public element<item_subtype_list>
	{
	public:
		id<item> const id;

		virtual ~item() = default;

		//! The item's name.
		virtual std::string name() const = 0;

		//! The item's weight.
		virtual double weight() const = 0;

		//! The list of actions that can be performed with the item.
		virtual std::vector<uptr<action>> actions() = 0;

		//! Advances the item one time unit.
		virtual void update() {}
	protected:
		class drop : public action
		{
		public:
			drop(item& item) : _item{item} {}
			static auto make(item& item) { return umake<drop>(item); }
			std::string name() const final { return "Drop"; }
			complete perform(being& actor, cont cont) final;
		private:
			item& _item;
		};

		class toss : public action
		{
		public:
			toss(item& item) : _item{item} {}
			static auto make(item& item) { return umake<toss>(item); }
			std::string name() const final { return "Throw"; }
			complete perform(being& actor, cont cont) final;
		private:
			item& _item;
		};

		item(ql::id<item> id) : id{id} {}
	};

	DEFINE_ELEMENT_BASE(item, item)
}
