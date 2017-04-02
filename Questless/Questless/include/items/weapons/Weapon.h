/**
* @file    Weapon.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Weapon abstract base class.
*/

#pragma once

#include "items/Equipment.h"
#include "items/Breakable.h"
#include "items/weapons/Attack.h"
#include "entities/beings/Damage.h"

namespace questless
{
	class Weapon : public Equipment, public Breakable
	{
	public:
		using uptr = std::unique_ptr<Weapon>;
		using ref = std::reference_wrapper<Weapon>;
		using cref = std::reference_wrapper<Weapon const>;

		virtual ~Weapon() = default;

		/// Time left before the weapon can be used again.
		////
		double active_cooldown = 0.0; /// @todo Replace with a non-negative double property.
	protected:
		Weapon(double integrity) : Breakable{integrity} {}
	};
}
