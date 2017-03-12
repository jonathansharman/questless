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
		actor.take_item(_item.id);

		if (ItemBox* old_box = dynamic_cast<ItemBox*>(actor.region->object(actor.coords))) {
			// If there's already an item box on the ground at the drop location, put the item in it.
			old_box->items().push_back(_item.id);
			return cont(Result::success);
		} else {
			// Otherwise make a new box to put it in.
			auto new_box = std::make_unique<ItemBox>();
			new_box->items().push_back(_item.id);
			actor.region->add<Object>(std::move(new_box), actor.coords);
			return cont(Result::success);
		}
	}
}
