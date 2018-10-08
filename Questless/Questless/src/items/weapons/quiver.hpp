//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "items/inventory.hpp"

namespace ql {
	//! An item for containing projectiles, such as arrows.
	class quiver : public equipment_base<quiver> {
	public:
		inventory inventory;

		quiver(ql::inventory inventory, ql::id<item> id = ql::id<item>::make());

		std::string name() const final { return "Quiver"; }

		load mass() const final { return 0.5_load; }

		std::vector<uptr<action>> actions() final;

		tick equip_time() const final { return 1_tick; }
		tick unequip_time() const final { return 1_tick; }
	private:
		body_part_counts requirements() const final { return body_part_counts{torsos{1}}; }
	};
}
