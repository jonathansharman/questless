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
	Action::Complete Item::Drop::perform(Being& actor, cont_t cont)
	{
		Item::ptr item = actor.take_item(_item);

		// See if there's already an item box on the ground at the drop location.
		if (Object* object = actor.region().object(actor.coords())) {
			if (ItemBox* item_box = dynamic_cast<ItemBox*>(object)) {
				item_box->items().push_back(std::move(item));
				return cont(Result::success);
			}
		}
		// Otherwise make a new box and add to it.
		auto item_box = std::make_unique<ItemBox>(actor.game(), ObjectId::next());
		item_box->items().push_back(std::move(item));
		actor.region().add<Object>(std::move(item_box), actor.coords());
		return cont(Result::success);
	}
}
