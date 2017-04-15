/**
* @file    Bow.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "items/weapons/Bow.h"
#include "items/weapons/Arrow.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	/// @todo These functions are O(n). Should be easy to find an efficient O(1) solution.

	Complete Bow::Fire::ArrowCost::check(Being& actor, cont_t cont) const
	{
		for (Item& maybe_quiver : actor.inventory.items()) {
			if (Quiver* quiver = dynamic_cast<Quiver*>(&maybe_quiver)) {
				if (quiver->equipped() && quiver->bearer_id() == actor.id) {
					for (Item& maybe_arrow : quiver->inventory.items()) {
						if (dynamic_cast<Arrow*>(&maybe_arrow)) {
							// Found an arrow in an equipped quiver.
							return cont();
						}
					}
				}
			}
		}
		for (Item& item : actor.inventory.items()) {
			if (dynamic_cast<Arrow*>(&item)) {
				// Found an arrow free-floating in the inventory.
				return cont();
			}
		}
		return actor.agent().send_message(std::make_unique<MessageNotEnoughAmmo>(), [cont] { return Complete{}; });
	}

	void Bow::Fire::ArrowCost::incur(Being& actor) const
	{
		for (Item& maybe_quiver : actor.inventory.items()) {
			if (Quiver* quiver = dynamic_cast<Quiver*>(&maybe_quiver)) {
				if (quiver->equipped() && quiver->bearer_id() == actor.id) {
					for (Item& maybe_arrow : quiver->inventory.items()) {
						if (Arrow* arrow = dynamic_cast<Arrow*>(&maybe_arrow)) {
							// Found an arrow in an equipped quiver.
							quiver->inventory.remove(arrow->id);
							return;
						}
					}
				}
			}
		}
		for (Item& item : actor.inventory.items()) {
			if (dynamic_cast<Arrow*>(&item)) {
				// Found an arrow free-floating in the inventory.
				actor.inventory.remove(item.id);
			}
		}
		throw std::logic_error{"Couldn't find arrow to spend."};
	}
}
