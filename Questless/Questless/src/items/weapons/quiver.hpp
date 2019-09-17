//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "items/inventory.hpp"
#include "reg.hpp"

namespace ql {
	//! An item for containing projectiles, such as arrows.
	struct quiver {
		id id;
		inventory inventory;
	};

	auto make_quiver(id quiver_id, inventory inventory) -> id;
}
