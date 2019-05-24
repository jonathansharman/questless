//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql {
	//! Component for items that can be used for attacking.
	struct weapon {
		//! Time left before the weapon can be used again.
		nonnegative<tick> active_cooldown = 0_tick;
	};
}
