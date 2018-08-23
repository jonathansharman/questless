//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/quarterstaff.hpp"

namespace ql {
	quarterstaff::quarterstaff(ql::id<item> id)
		: item{id}
		, modal_weapon_base<quarterstaff>{durability(), umake<standard_form>(*this)}
	{}

	std::vector<uptr<action>> quarterstaff::standard_form::actions() {
		std::vector<uptr<action>> actions;
		if (weapon().equipped()) {
			actions.push_back(_strike->launch());
			actions.push_back(_jab->launch());
			actions.push_back(switch_form<half_staff_form>::make(weapon(), "Switch to half-staff grip"));
			actions.push_back(unequip::make(weapon()));
		} else {
			actions.push_back(equip::make(weapon()));
			actions.push_back(drop::make(weapon()));
			actions.push_back(toss::make(weapon()));
		}
		return actions;
	}

	std::vector<uptr<action>> quarterstaff::half_staff_form::actions() {
		std::vector<uptr<action>> actions;
		if (weapon().equipped()) {
			actions.push_back(_strike->launch());
			actions.push_back(_jab->launch());
			actions.push_back(switch_form<standard_form>::make(weapon(), "Switch to quarterstaff grip"));
			actions.push_back(unequip::make(weapon()));
		} else {
			actions.push_back(equip::make(weapon()));
			actions.push_back(drop::make(weapon()));
			actions.push_back(toss::make(weapon()));
		}
		return actions;
	}
}
