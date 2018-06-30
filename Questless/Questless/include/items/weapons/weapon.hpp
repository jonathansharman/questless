//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "items/breakable.hpp"
#include "items/weapons/attack.hpp" // For convenience; used by virtually all weapons.

namespace ql
{
	//! An item that can be used for attacking.
	class weapon : public equipment, public breakable
	{
	public:
		virtual ~weapon() = default;

		//! Time left before the weapon can be used again.
		double active_cooldown = 0.0; //! @todo Replace with a non-negative double property.
	protected:
		weapon(double integrity) : breakable{integrity} {}
	};

	DEFINE_ELEMENT_BASE(weapon, item)
}
