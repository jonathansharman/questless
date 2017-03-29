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
	void Armor::apply(Damage& damage)
	{
		// Broken armor has no effect.
		if (!broken()) {
			// Calcuate reduced damage by first applying protection and then applying resistance.
			Damage const reduced_damage = damage
				.with(protection()) // First apply protection.
				.with(resistance(), Vulnerability::zero()); // Then apply resistance.

			// Calculate the armor's total effective damage reduction.
			double const effective_reduction = damage.total() - reduced_damage.total();

			// Set damage to its reduced value and cause the armor to wear in proportion to its effective damage reduction.
			damage = reduced_damage;
			integrity -= wear_ratio() * effective_reduction;
		}
	}
}
