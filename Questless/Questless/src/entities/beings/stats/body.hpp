//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/armor.hpp"

namespace ql::stats {
	//! The stats of a being as a whole.
	struct body {
		aggregate a{};
		stat<dmg::armor> armor;

		//! Resets current stats to base stats.
		auto reset() -> void {
			a.reset();
			armor.reset();
		}
	};
}
