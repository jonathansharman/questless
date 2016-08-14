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
	Damage Armor::apply(Damage damage)
	{
		// Broken armor has no effect.
		if (broken()) { return damage; }

		Damage max_reduction = protection().reduction();
		Damage reduced_damage = damage - max_reduction;
		Damage effective_reduction = damage - reduced_damage;
		wear(wear_ratio() * effective_reduction.total());
		return reduced_damage;
	}
}
