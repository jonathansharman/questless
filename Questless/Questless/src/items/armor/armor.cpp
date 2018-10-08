//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "damage/group.hpp"
#include "items/armor/armor.hpp"

namespace ql {
	void armor::apply(dmg::group& damage) {
		// Broken armor has no effect.
		if (!broken()) {
			// Calcuate reduced damage.
			dmg::group const reduced_damage = damage.with(protection(), resistance(), dmg::vuln::zero());

			// Calculate the armor's total effective damage reduction.
			health const effective_reduction = damage.health_loss() - reduced_damage.health_loss();

			// Set damage to its reduced value and cause the armor to wear in proportion to its effective damage reduction.
			damage = reduced_damage;
			integrity -= wear_ratio() * effective_reduction;
		}
	}
}
