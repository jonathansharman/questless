/**
* @file    Inventory.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Inventory class.
*/

#include "items/Inventory.h"

#include "Game.h"

namespace questless
{
	void Inventory::add(Id<Item> item_id)
	{
		// Search existing pages for an empty slot.
		for (size_t page = 0; page < _pages.size(); ++page) {
			for (size_t row = 0; row < Page::rows; ++row) {
				for (size_t column = 0; column < Page::columns; ++column) {
					if (_pages[page].item_ids[row][column] == boost::none) {
						_pages[page].item_ids[row][column] = item_id;
						return;
					}
				}
			}
		}
		// If full, add to new page.
		_pages.push_back(Page{});
		_pages.back().item_ids[0][0] = item_id;
	}

	void Inventory::remove(Id<Item> item_id)
	{
		for (int page = 0; page < static_cast<int>(_pages.size()); ++page) {
			for (int row = 0; row < Page::rows; ++row) {
				for (int column = 0; column < Page::columns; ++column) {
					if (auto opt_id = _pages[page].item_ids[row][column]) {
						if (*opt_id == item_id) {
							remove(Coords{page, row, column});
							return;
						}
					}
				}
			}
		}
	}

	void Inventory::remove(Coords coords)
	{
		_pages[coords.page].item_ids[coords.row][coords.column] = boost::none;
	}

	Item* Inventory::get(Coords coords)
	{
		auto opt_item_id = _pages[coords.page].item_ids[coords.row][coords.column];
		return opt_item_id
			? game().items[*opt_item_id]
			: nullptr
			;
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
				( _pages[page].item_ids[row_start][column_start] == boost::none
					? Stream<Item*>{}
					: singleton<Item*>(game().items[*_pages[page].item_ids[row_start][column_start]])
				, items(page, row_start, column_start + 1)
				);
	}
}
