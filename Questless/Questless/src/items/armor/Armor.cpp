/**
* @file    Armor.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Armor abstract base class.
*/

#include "items/armor/Armor.h"

namespace questless
{
	void Armor::apply_protection(Damage& damage)
	{
		if (!broken()) { // Broken armor has no effect.
			// Compute the armor's effective damage reduction.
			Damage const reduced_damage = damage.with(protection());
			double const effective_reduction = damage.total() - reduced_damage.total();

			// Set damage to its reduced value and cause the armor to wear in proportion to its effective damage reduction.
			damage = reduced_damage;
			integrity -= wear_ratio() * effective_reduction;
		}
	}

	void Armor::take_resistance_wear(Damage const& damage)
	{
		double max_reduction = damage.total() - damage.with(resistance(), Vulnerability::zero()).total();
		integrity -= wear_ratio() * max_reduction;
	}
}
