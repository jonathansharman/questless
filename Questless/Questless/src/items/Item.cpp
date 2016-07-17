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
#include "entities/beings/Agent.h"

#include "Game.h"
#include "entities/objects/ItemBox.h"

namespace questless
{
	void Item::Drop::perform(Game& game, Being& actor, cont_t cont)
	{
		auto item_box = make_unique<ItemBox>(Entity::next_id());
		Item::ptr item = actor.take_item(_item);
		/// @todo item_box->add(std::move(item));
		game.region().add<Object>(std::move(item_box), actor.coords());
		cont(Result::success);
	}
}
