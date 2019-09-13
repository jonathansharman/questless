//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quarterstaff.hpp"

#include "items/breakable.hpp"
#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	auto quarterstaff::strike() -> void {
		//! @todo This.

		dmg::group damage = {24_bludgeon};
		tick cooldown = 20_tick;
	}

	auto quarterstaff::jab() -> void {
		//! @todo This.

		dmg::group damage = {12_bludgeon};
		tick cooldown = 13_tick;
	}

	auto make_quarterstaff(id quarterstaff_id) -> id {
		make_item(quarterstaff_id, 2.5_mass);

		make_breakable(quarterstaff_id, {500_durability, 500_durability});

		make_equipment(quarterstaff_id,
			std::nullopt,
			{equipment::tab{body_part::tag::hand, std::nullopt}, equipment::tab{body_part::tag::hand, std::nullopt}},
			1_ap,
			1_ap);

		reg.assign<quarterstaff>(quarterstaff_id, quarterstaff_id);

		return quarterstaff_id;
	}
}
