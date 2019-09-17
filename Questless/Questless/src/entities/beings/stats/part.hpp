//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/armor.hpp"

namespace ql::stats {
	//! Stats belonging only to a particular body part.
	struct part {
		aggregate a;

		stat<int> regen_factor;
		stat<blood_per_tick> bleeding;
		stat<ql::temperature> min_temp;
		stat<ql::temperature> max_temp;
		stat<dmg::armor> armor;

		auto blood_regen() const -> blood_per_tick {
			// Blood regen is proportional to base vitality and the regen factor.
			return a.vitality.base * regen_factor.cur * 0.01_blood / 1_tick / 1_hp;
		}

		//! Resets current stats to base stats.
		auto reset() -> void {
			a.reset();
			regen_factor.reset();
			bleeding.reset();
			min_temp.reset();
			max_temp.reset();
			armor.reset();
		}
	};
}
