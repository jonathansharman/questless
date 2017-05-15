//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/Inventory.h"

#include "Game.h"

namespace questless
{
	ref<Item> Inventory::item_id_to_ref(Id<Item> item_id)
	{
		return game().items.ref(item_id);
	}
	cref<Item> Inventory::item_id_to_cref(Id<Item> item_id)
	{
		return game().items.cref(item_id);
	}
}
