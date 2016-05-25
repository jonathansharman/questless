/**
* @file    Equipable.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Equipable abstract base class, for items that can be equiped on a being.
*/

#ifndef EQUIPABLE_H
#define EQUIPABLE_H

#include "items/Item.h"

namespace questless
{
	class Equipable : public virtual Item
	{
	public:
		virtual ~Equipable() = default;

		/// @return The amount of time required to equip the item.
		virtual double equip_time() const = 0;

		/// @return The amount of time required to unequip the item.
		virtual double unequip_time() const = 0;

		/// @todo Not general enough! Weapons could be equipped on anything.
		virtual int hands() const = 0; ///< The number of hands required to wield the weapon.
	};
}

#endif