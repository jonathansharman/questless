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

		double weight() const final { return 0.5; }

		std::vector<uptr<action>> actions() final;

		double equip_time() const final { return 1.0; }
		double unequip_time() const final { return 1.0; }
	private:
		body_part_counts requirements() const final { return body_part_counts{torsos{1}}; }
	};
}
