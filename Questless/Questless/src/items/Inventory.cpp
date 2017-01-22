/**
* @file    Inventory.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Inventory class.
*/

#include "items/Inventory.h"

namespace questless
{
	void Inventory::add(Item::ptr item)
	{
		// Search existing pages for an empty slot.
		for (size_t page = 0; page < _pages.size(); ++page) {
			for (size_t row = 0; row < Page::rows; ++row) {
				for (size_t column = 0; column < Page::columns; ++column) {
					if (_pages[page].items[row][column] == nullptr) {
						_pages[page].items[row][column] = std::move(item);
						return;
					}
				}
			}
		}
		// If full, add to new page.
		_pages.push_back(Page{});
		_pages.back().items[0][0] = std::move(item);
	}

	Item::ptr Inventory::remove(const Item& item)
	{
		for (size_t page = 0; page < _pages.size(); ++page) {
			for (size_t row = 0; row < Page::rows; ++row) {
				for (size_t column = 0; column < Page::columns; ++column) {
					if (_pages[page].items[row][column].get() == &item) {
						return remove(Coords{page, row, column});
					}
				}
			}
		}
		return nullptr;
	}

	Item::ptr Inventory::remove(Coords coords)
	{
		Item::ptr removed_item = std::move(_pages[coords.page].items[coords.row][coords.column]);
		return removed_item;
	}

	Stream<Item*> Inventory::items(size_t page_start)
	{
		return page_start == _pages.size()
			? Stream<Item*>{}
			: sconcat(items(page_start, 0), items(page_start + 1));
	}

	Stream<Item*> Inventory::items(size_t page, size_t row_start)
	{
		return row_start == Page::rows
			? Stream<Item*>{}
			: sconcat(items(page, row_start, 0), items(page, row_start + 1));
	}

	Stream<Item*> Inventory::items(size_t page, size_t row_start, size_t column_start)
	{
		return column_start == Page::columns
			? Stream<Item*>{}
			: sconcat
				( _pages[page].items[row_start][column_start] == nullptr
					? Stream<Item*>{}
					: singleton<Item*>(_pages[page].items[row_start][column_start].get())
				, items(page, row_start, column_start + 1)
				);
	}
}
