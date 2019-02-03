//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_set>

#include "utility/container_view.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

namespace ql {
	struct item;

	//! Represents a physical collection of items, e.g. the items in a being's possession.
	struct inventory {
		static ref<item> item_id_to_ref(id<item> item_id);
		static cref<item> item_id_to_cref(id<item> item_id);
		using items_view = container_view
			< std::unordered_set<id<item>>
			, ref<item>
			, cref<item>
			, item_id_to_ref
			, item_id_to_cref
			>;

		inventory() : items{_item_ids} {}
		inventory(inventory const&) = delete;
		inventory(inventory&& that) noexcept : items{_item_ids}, _item_ids{std::move(that._item_ids)} {}

		//! The set of IDs of items in the inventory.
		std::unordered_set<id<item>> const& item_ids() const { return _item_ids; }

		//! The set of items in the inventory.
		items_view items;

		//! Adds the item with the given ID to the inventory.
		//! @param item_id The ID of the item to add to the inventory.
		void add(id<item> item_id) { _item_ids.insert(item_id); }

		//! Removes the item with the given ID from the inventory.
		//! @param item_id The ID of an item to remove from the inventory.
		void remove(id<item> item_id) { _item_ids.erase(item_id); }
	private:
		std::unordered_set<id<item>> _item_ids;
	};
}
