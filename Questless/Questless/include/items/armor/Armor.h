//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Equipment.h"
#include "items/Breakable.h"
#include "entities/beings/Protection.h"
#include "entities/beings/DamageMultiplier.h"

namespace questless
{
	//! Abstract base class for items that serve as armor.
	class Armor : public virtual Equipment, public virtual Breakable
	{
	public:
		virtual ~Armor() = default;

		//! The armor's effective protection per hit, accounting for wear.
		Protection protection() { return base_protection() * (1.0 + integrity / durability() / 2.0); }

		//! The base amount of protection the armor provides.
		virtual Protection base_protection() const = 0;

		//! The armor's effective damage resistance, accounting for wear.
		Resistance resistance() { return base_resistance() * (1.0 + integrity / durability() / 2.0); }

		//! The base amount of damage resistance the armor provides.
		virtual Resistance base_resistance() const = 0;

		//! The proportion of the damage from which this armor protects to be applied as wear.
		virtual double wear_ratio() const = 0;

		//! Applies the armor's protection and resistance to the given damage, possibly reducing the damage and causing the armor to wear.
		void apply(Damage& damage);
	};
}
