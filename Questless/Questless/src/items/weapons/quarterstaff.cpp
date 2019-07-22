//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quarterstaff.hpp"

#include "items/breakable.hpp"
#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	void quarterstaff::strike() {
		//! @todo This.

		dmg::group damage = 24_bludgeon;
		tick cooldown = 20_tick;
	}

	void quarterstaff::jab() {
		//! @todo This.

		dmg::group damage = 12_bludgeon;
		tick cooldown = 13_tick;
	}

	void make_quarterstaff(ent id) {
		make_item(id, 2.5_mass);

		make_breakable(id, 500.0_integrity, 500.0_integrity);

		make_equipment(id,
			std::nullopt,
			{equipment::tab{body_part::tag::hand, std::nullopt}, equipment::tab{body_part::tag::hand, std::nullopt}},
			1_tick, // Equip time
			1_tick); // Unequip time

		reg.assign<quarterstaff>(id, id);
	}
}
