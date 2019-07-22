//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/armor.hpp"

namespace ql::stats {
	//! Stats belonging only to a particular body part.
	struct part {
		aggregate a{};

		nonnegative<int> regen_factor = 0;
		nonnegative<blood_per_tick> bleeding = 0.0_blood_per_tick;
		ql::temperature min_temp = 0.0_temp;
		ql::temperature max_temp = 0.0_temp;
		dmg::armor armor{};

		blood_per_tick blood_regen() const {
			// Blood regen is proportional to maximum vitality and the regen factor.
			return 0.01_blood / 1_tick / 1_hp * a.vitality.upper_bound() * regen_factor.value();
		}
	};
}
