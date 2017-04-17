//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/Inventory.h"

#include "Game.h"

namespace questless
{
	Item::ref Inventory::Items::item_id_to_ref(Id<Item> item_id)
	{
		return game().items.get_ref(item_id);
	}
	Item::cref Inventory::Items::item_id_to_cref(Id<Item> item_id)
	{
		return game().items.get_ref(item_id);
	}
}
