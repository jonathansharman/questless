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
		virtual ~Armor() = default;

		/// @return The armor's effective protection per hit, accounting for wear.
		Protection protection() { return base_protection() * (1.0 + integrity() / durability() / 2.0); }

		/// @return The base amount of protection the armor provides.
		virtual Protection base_protection() const = 0;

		/// @return The proportion of 
		virtual double wear_ratio() const = 0;

		/// Applies the given damage to the armor and returns the reduced damage.
		/// @param damage The damage applied to the armor.
		/// @return The reduced damage, accounting for the armor's protection.
		Damage apply(Damage damage)
		{
			// Broken armor has no effect.
			if (broken()) { return damage; }

			Damage reduction = protection().reduction();
			Damage reduced_damage = damage - reduction;
			Damage effective_reduction = damage - reduced_damage;
			wear(wear_ratio() * effective_reduction.total());
			return reduced_damage;
		}
	};
}

#endif
