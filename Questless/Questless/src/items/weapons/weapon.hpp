//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/breakable.hpp"
#include "items/equipment.hpp"
#include "items/weapons/attack.hpp" // For convenience; used by virtually all weapons.

namespace ql {
	//! An item that can be used for attacking.
	struct weapon
		: equipment
		, breakable {
		virtual ~weapon() = default;

		//! Time left before the weapon can be used again.
		nonnegative<tick> active_cooldown = 0_tick;

	protected:
		weapon(ql::integrity integrity) : breakable{integrity} {}
	};

	DEFINE_ELEMENT_BASE(weapon, item)
}