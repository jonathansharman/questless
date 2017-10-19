//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "items/inventory.hpp"

namespace ql
{
	//! An item for containing projectiles, such as arrows.
	class quiver : public equipment_base<quiver>
	{
	public:
		inventory inventory;

		quiver(ql::inventory inventory, ql::id<item> id = ql::id<item>::make())
			: item{id}
			, inventory{std::move(inventory)}
		{}

		std::string name() const final { return "Quiver"; }

		double weight() const final { return 0.5; }

		virtual std::vector<uptr<action>> actions() final
		{
			std::vector<uptr<action>> actions;
			if (equipped()) {
				actions.push_back(unequip::make(*this));
			} else {
				actions.push_back(equip::make(*this));
				actions.push_back(drop::make(*this));
				actions.push_back(toss::make(*this));
			}
			return actions;
		}

		double equip_time() const final { return 1.0; }
		double unequip_time() const final { return 1.0; }
	private:
		body_part_counts requirements() const final { return body_part_counts{torsos{1}}; }
	};
}
