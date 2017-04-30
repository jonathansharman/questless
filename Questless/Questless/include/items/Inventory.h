//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_set>

#include "utility/Id.h"
#include "utility/XformedContainer.h"
#include "utility/reference.h"

namespace questless
{
	class Item;

	//! Represents a physical collection of items, e.g. the items in a being's possession.
	class Inventory
	{
	public:
		static ref<Item> item_id_to_ref(Id<Item> item_id);
		static cref<Item> item_id_to_cref(Id<Item> item_id);
		using ItemsView = XformedContainer
			< std::unordered_set<Id<Item>>
			, ref<Item>
			, cref<Item>
			, item_id_to_ref
			, item_id_to_cref
			>;

		Inventory() : items{_item_ids} {}
		Inventory(Inventory const&) = delete;
		Inventory(Inventory&& that) : items{_item_ids}, _item_ids{std::move(that._item_ids)} {}

		//! The set of IDs of items in the inventory.
		std::unordered_set<Id<Item>> const& item_ids() const { return _item_ids; }

		//! The set of items in the inventory.
		ItemsView items;

		//! Adds the item with the given ID to the inventory.
		//! @param item_id The ID of the item to add to the inventory.
		void add(Id<Item> item_id) { _item_ids.insert(item_id); }

		//! Removes the item with the given ID from the inventory.
		//! @param item_id The ID of an item to remove from the inventory.
		void remove(Id<Item> item_id) { _item_ids.erase(item_id); }
	private:
		std::unordered_set<Id<Item>> _item_ids;
	};
}
