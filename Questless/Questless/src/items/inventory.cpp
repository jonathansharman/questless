//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/inventory.hpp"

#include "game.hpp"

namespace ql {
	ref<item> inventory::item_id_to_ref(id<item> item_id) {
		return the_game().items.ref(item_id);
	}
	cref<item> inventory::item_id_to_cref(id<item> item_id) {
		return the_game().items.cref(item_id);
	}
}
