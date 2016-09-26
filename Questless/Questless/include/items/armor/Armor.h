/**
* @file    Armor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Armor class.
*/

#ifndef ARMOR_H
#define ARMOR_H

#include "items/Equipable.h"
#include "items/Breakable.h"
#include "entities/beings/Protection.h"

namespace questless
{
	class Armor : public virtual Equipable, public virtual Breakable
	{
	public:
		using ptr = std::unique_ptr<Armor>;
		using ref = std::reference_wrapper<Armor>;

		virtual ~Armor() = default;

		/// @return The armor's effective protection per hit, accounting for wear.
		Protection protection() { return base_protection() * (1.0 + integrity() / durability() / 2.0); }

		/// @return The base amount of protection the armor provides.
		virtual Protection base_protection() const = 0;

		/// @return The proportion of the damage from which this armor protects to be applied as wear.
		virtual double wear_ratio() const = 0;

		/// Applies the given damage to the armor and returns the reduced damage.
		/// @param damage The damage applied to the armor.
		/// @return The reduced damage, accounting for the armor's protection.
		Damage apply(Damage damage);
	};
}

#endif
