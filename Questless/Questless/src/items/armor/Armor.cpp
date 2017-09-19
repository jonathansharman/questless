//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/armor/Armor.h"

namespace questless
{
	void Armor::apply(dmg::Group& damage)
	{
		// Broken armor has no effect.
		if (!broken()) {
			// Calcuate reduced damage by first applying protection and then applying resistance.
			dmg::Group const reduced_damage = damage
				.with(protection()) // First apply protection.
				.with(resistance(), dmg::Vuln::zero()); // Then apply resistance.

			// Calculate the armor's total effective damage reduction.
			double const effective_reduction = damage.total() - reduced_damage.total();

			// Set damage to its reduced value and cause the armor to wear in proportion to its effective damage reduction.
			damage = reduced_damage;
			integrity -= wear_ratio() * effective_reduction;
		}
	}
}
