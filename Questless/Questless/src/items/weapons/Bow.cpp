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
	Complete Bow::Fire::ArrowCost::check(Being& actor, cont_t cont) const
	{
		if (Bow* bow = game().items.get_as<Bow>(_bow_id)) {
			if (std::optional<Id<Item>> quiver_id = bow->_quiver_id) {
				if (Quiver* quiver = game().items.get_as<Quiver>(*quiver_id)) {
					/// @todo Check that quiver is still equipped.
					for (Item* item : quiver->inventory.items()) {
						if (dynamic_cast<Arrow*>(item)) {
							// Found an arrow in the quiver.
							return cont();
						}
					}
				}
			}
		}
		for (Item* item : actor.inventory.items()) {
			if (dynamic_cast<Arrow*>(item)) {
				// Found an arrow free-floating in the inventory.
				return cont();
			}
		}
		return actor.agent().send_message(std::make_unique<MessageNotEnoughAmmo>(), [cont] { return Complete{}; });
	}

	void Bow::Fire::ArrowCost::incur(Being& actor) const
	{
		/// @todo This function is O(n). Should be easy to find a ~O(1) solution.
		if (Bow* bow = game().items.get_as<Bow>(_bow_id)) {
			if (std::optional<Id<Item>> quiver_id = bow->_quiver_id) {
				if (Quiver* quiver = game().items.get_as<Quiver>(*quiver_id)) {
					for (Item* item : quiver->inventory.items()) {
						if (dynamic_cast<Arrow*>(item)) {
							// Found an arrow in the quiver.
							quiver->inventory.remove(item->id);
							return;
						}
					}
				}
			}
		}
		for (Item* item : actor.inventory.items()) {
			if (dynamic_cast<Arrow*>(item)) {
				// Found an arrow free-floating in the inventory.
				actor.inventory.remove(item->id);
			}
		}
		throw std::logic_error{"Couldn't find arrow to spend."};
	}
}
