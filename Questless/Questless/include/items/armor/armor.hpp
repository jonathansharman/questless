//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "items/breakable.hpp"
#include "damage/group.hpp"
#include "damage/multiplier.hpp"

namespace ql
{
	//! Abstract base class for items that serve as armor.
	class armor : public virtual equipment, public virtual breakable
	{
	public:
		virtual ~armor() = default;

		//! The armor's effective protection per hit, accounting for wear.
		dmg::protect protection() { return base_protection() * (1.0 + integrity / durability() / 2.0); }

		//! The base amount of protection the armor provides.
		virtual dmg::protect base_protection() const = 0;

		//! The armor's effective damage resistance, accounting for wear.
		dmg::resist resistance() { return base_resistance() * (1.0 + integrity / durability() / 2.0); }

		//! The base amount of damage resistance the armor provides.
		virtual dmg::resist base_resistance() const = 0;

		//! The proportion of the damage from which this armor protects to be applied as wear.
		virtual double wear_ratio() const = 0;

		//! Applies the armor's protection and resistance to the given damage, possibly reducing the damage and causing the armor to wear.
		void apply(dmg::group& damage);
	};
}
