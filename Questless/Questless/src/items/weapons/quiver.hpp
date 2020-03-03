//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "items/inventory.hpp"
#include "reg.hpp"

namespace ql {
	//! An item for containing projectiles, such as arrows.
	struct quiver {
		reg_ptr reg;
		id id;
		inventory inventory;
	};

	auto make_quiver(reg& reg, id quiver_id, inventory inventory) -> id;
}
