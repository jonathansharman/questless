//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/inventory.hpp"
#include "reg.hpp"

namespace ql {
	//! An item for containing projectiles, such as arrows.
	struct quiver {
		id id;
		inventory inventory;
	};

	id make_quiver(id id, inventory inventory);
}
