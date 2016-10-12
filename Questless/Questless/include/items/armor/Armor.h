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
#include "entities/beings/DamageMultiplier.h"

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

		/// @return The armor's effective damage resistance, accounting for wear.
		Resistance resistance() { return base_resistance() * (1.0 + integrity() / durability() / 2.0); }

		/// @return The base amount of damage resistance the armor provides.
		virtual Resistance base_resistance() const = 0;

		/// @return The proportion of the damage from which this armor protects to be applied as wear.
		virtual double wear_ratio() const = 0;

		/// Applies the armor's protection to the given damage, possibly reducing the damage and causing the armor to wear.
		/// @param damage The damage to be reduced according to the armor's protection.
		void apply_protection(Damage& damage);

		/// Causes the armor to wear according to its maximum possible reduction from resistance to the given damage.
		/// @note It is the caller's responsibility to ensure the armor's resistance is actually applied to this damage. This function does not reduce the damage itself because actual damage reduction from resistance can only be computed given total resistance and vulnerability and only after all protection has been applied.
		void take_resistance_wear(const Damage& damage);
	};
}

#endif
