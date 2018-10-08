//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "items/breakable.hpp"
#include "damage/multiplier.hpp"

namespace ql {
	namespace dmg {
		class group;
	}

	//! Abstract base class for items that serve as armor.
	class armor : public virtual equipment, public virtual breakable {
	public:
		virtual ~armor() = default;

		//! The armor's effective protection per hit, accounting for wear.
		dmg::protect protection() { return base_protection() * (0.5 + 0.5 * (integrity.value() / durability()).value); }

		//! The base amount of protection the armor provides.
		virtual dmg::protect base_protection() const = 0;

		//! The armor's effective damage resistance, accounting for wear.
		dmg::resist resistance() { return base_resistance() * (0.5 + 0.5 * (integrity.value() / durability()).value); }

		//! The base amount of damage resistance the armor provides.
		virtual dmg::resist base_resistance() const = 0;

		//! Reduction in integrity per HP saved by this armor.
		virtual decltype(ql::integrity{} / ql::health{}) wear_ratio() const = 0;

		//! Applies the armor's protection and resistance to the given damage, possibly reducing the damage and causing the armor to wear.
		void apply(dmg::group& damage);
	};
}
