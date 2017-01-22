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
			Damage max_reduction = protection().reduction();
			Damage reduced_damage = damage - max_reduction;
			Damage effective_reduction = damage - reduced_damage;

			// Set damage to its reduced value and cause the armor to wear in proportion to its effective damage reduction.
			damage = reduced_damage;
			wear(wear_ratio() * effective_reduction.total());
		}
	}

	void Armor::take_resistance_wear(Damage const& damage)
	{
		Damage max_reduction = damage - damage.with(resistance(), Vulnerability::zero());
		wear(wear_ratio() * max_reduction.total());
	}
}
