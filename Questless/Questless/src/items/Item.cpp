/**
* @file    Item.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Item abstract base class.
*/

#include "items/Item.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

#include "Game.h"
#include "entities/objects/ItemBox.h"

namespace questless
{
	Complete Item::Drop::perform(Being& actor, cont_t cont)
	{
		actor.inventory.remove(_item.id);

		if (Object* object = actor.region->object(actor.coords)) {
			if (ItemBox* old_box = dynamic_cast<ItemBox*>(object)) {
				// There's already an item box on the ground at the drop location. Put the item in it.
				old_box->items().push_back(_item.id);
				return cont(Result::success);
			} else {
				// There's another object in the way at the drop location.
				return actor.agent().send_message(std::make_unique<MessageEntityInTheWay>(), [cont] { return cont(Result::aborted); });

				/// @todo It should probably always be possible to drop stuff. This by itself is a good reason to allow multiple objects on a tile.
			}
		} else {
			// Nothing on the ground yet. Make a new box, and put the item in it.
			auto new_box = std::make_unique<ItemBox>();
			new_box->items().push_back(_item.id);
			actor.region->spawn(std::move(new_box), actor.coords);
			return cont(Result::success);
		}
	}

	Complete Item::Throw::perform(Being& actor, cont_t cont)
	{
		/// @todo This.
		auto drop = Drop{_item};
		return drop.perform(actor, cont);
	}
}
