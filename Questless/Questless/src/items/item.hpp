//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/action.hpp"
#include "item_visitor.hpp"
#include "quantities/mass.hpp"
#include "quantities/quantities.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

namespace ql {
	struct being;

	//! An item that can be picked up, used by beings, etc.
	struct item : element<item_subtype_list> {
		id<item> const id;

		virtual ~item() = default;

		//! The item's name.
		virtual std::string name() const = 0;

		//! The item's mass.
		virtual mass mass() const = 0;

		//! The list of actions that can be performed with the item.
		virtual std::vector<uptr<action>> actions() = 0;

		//! Advances this item by @p elapsed.
		virtual void update(tick elapsed);

	protected:
		struct drop : action {
				drop(item& item) : _item{item} {}
			static auto make(item& item) {
				return umake<drop>(item);
			}
			std::string name() const final {
				return "Drop";
			}
			complete perform(being& actor, cont cont) final;

		private:
			item& _item;
		};

		struct toss : action {
				toss(item& item) : _item{item} {}
			static auto make(item& item) {
				return umake<toss>(item);
			}
			std::string name() const final {
				return "Throw";
			}
			complete perform(being& actor, cont cont) final;

		private:
			item& _item;
		};

		item(ql::id<item> id) : id{id} {}
	};

	DEFINE_ELEMENT_BASE(item, item)
}
