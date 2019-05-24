//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

#include <unordered_set>

namespace ql {
	struct item;

	//! Represents a physical collection of items, e.g. the items in a being's possession.
	struct inventory {
		//! The set of IDs of items in the inventory.
		std::unordered_set<ent> item_ids;

		//! Adds the item with the given ID to the inventory.
		//! @param item_id The ID of the item to add to the inventory.
		void add(ent item_id) {
			item_ids.insert(item_id);
		}

		//! Removes the item with the given ID from the inventory.
		//! @param item_id The ID of an item to remove from the inventory.
		void remove(ent item_id) {
			item_ids.erase(item_id);
		}
	};
}
