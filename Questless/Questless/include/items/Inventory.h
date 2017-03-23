/**
* @file    Inventory.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Inventory class.
*/

#pragma once

#include <vector>
#include <array>
#include <string>
#include <optional>

#include "Item.h"
#include "utility/Stream.h"

namespace questless
{
	class Inventory
	{
	public:
		struct Page
		{
			static constexpr int rows = 8;
			static constexpr int columns = 12;

			std::string label = "";

			std::array<std::array<std::optional<Id<Item>>, columns>, rows> item_ids;
		};

		struct Coords
		{
			int page;
			int row;
			int column;

			Coords() : page{0}, row{0}, column{0} {}
			Coords(int page, int row, int column) : page{page}, row{row}, column{column} {}
		};

		/// Adds the item with the given ID to the inventory in the first empty item slot.
		/// @param item_id The ID of the item to add to the inventory.
		////
		void add(Id<Item> item_id);

		/// Finds and removes the item with the given ID from the inventory.
		/// @param item_id The ID of an item to remove from the inventory.
		void remove(Id<Item> item_id);

		/// Removes the item at the given coordinates from the inventory.
		/// @param coords The inventory coordinates of the item to remove.
		void remove(Coords coords);

		/// @return A stream of items in the inventory, ordered by page and then coordinates.
		Stream<Item*> items() { return items(0); }

		/// @return The number of item pages in the inventory.
		int pages() const { return _pages.size(); }

		/// @return The inventory page at the given index.
		Page& page(int page_idx) { return _pages[page_idx]; }

		/// @return The item at the given inventory coordinates or nullptr if none present.
		/// @param coords Inventory coordinates (page, row, and column) of the requested item.
		Item* get(Coords coords);

		/// @return The item at the given inventory coordinates or nullptr if none present.
		/// @param coords Inventory coordinates (page, row, and column) of the requested item.
		Item* operator [](Coords coords) { return get(coords); }
	private:
		std::vector<Page> _pages;

		/// @todo Is the convenience of streams really worth this monstrosity?
		Stream<Item*> items(size_t page_start);
		Stream<Item*> items(size_t page, size_t row_start);
		Stream<Item*> items(size_t page, size_t row_start, size_t column_start);
	};
}
