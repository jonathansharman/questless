//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_set>

#include <boost/iterator/transform_iterator.hpp>

#include "Item.h"

namespace questless
{
	//! Represents a physical collection of items, e.g. the items in a being's possession.
	class Inventory
	{
		class Items
		{
		public:
			Items(std::unordered_set<Id<Item>>& item_ids) : _item_ids{item_ids} {}

			//! Iterator to the beginning of the item IDs.
			auto begin()
			{
				return boost::make_transform_iterator(_item_ids.begin(), item_id_to_ref);
			}
			//! Iterator to the end of the item IDs.
			auto end()
			{
				return boost::make_transform_iterator(_item_ids.end(), item_id_to_ref);
			}

			//! Const iterator to the beginning of item IDs.
			auto begin() const
			{
				return boost::make_transform_iterator(_item_ids.begin(), item_id_to_cref);
			}
			//! Const iterator to the end of the item IDs.
			auto end() const
			{
				return boost::make_transform_iterator(_item_ids.end(), item_id_to_cref);
			}

			//! The number of items.
			size_t size() { return _item_ids.size(); }
		private:
			std::unordered_set<Id<Item>>& _item_ids;

			static ref<Item> item_id_to_ref(Id<Item> item_id);
			static cref<Item> item_id_to_cref(Id<Item> item_id);
		};
	public:
		Inventory() : _items{_item_ids} {}
		Inventory(Inventory const& that) : _item_ids{that._item_ids}, _items{_item_ids} {}
		Inventory(Inventory&& that) : _item_ids{std::move(that._item_ids)}, _items{_item_ids} {}

		//! The set of IDs of items in the inventory.
		std::unordered_set<Id<Item>> const& item_ids() const { return _item_ids; }

		//! The set of items in the inventory.
		Items const& items() const { return _items; }
		//! The set of items in the inventory.
		Items& items() { return _items; }

		//! Adds the item with the given ID to the inventory.
		//! @param item_id The ID of the item to add to the inventory.
		void add(Id<Item> item_id) { _item_ids.insert(item_id); }

		//! Removes the item with the given ID from the inventory.
		//! @param item_id The ID of an item to remove from the inventory.
		void remove(Id<Item> item_id) { _item_ids.erase(item_id); }
	private:
		std::unordered_set<Id<Item>> _item_ids;
		Items _items;
	};
}
