//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Equipment.h"
#include "items/Breakable.h"
#include "items/weapons/Attack.h"
#include "entities/beings/Damage.h"

namespace questless
{
	//! An item that can be used for attacking.
	class Weapon : public Equipment, public Breakable
	{
	public:
		virtual ~Weapon() = default;

		//! Time left before the weapon can be used again.
		double active_cooldown = 0.0; //! @todo Replace with a non-negative double property.
	protected:
		Weapon(double integrity) : Breakable{integrity} {}
	};

	DEFINE_CONST_ELEMENT_BASE(Weapon, Item)
}
