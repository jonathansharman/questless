//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/item.hpp"
#include "entities/beings/being.hpp"
#include "agents/agent.hpp"

#include "game.hpp"
#include "entities/objects/item_box.hpp"

namespace ql
{
	complete item::drop::perform(being& actor, cont cont)
	{
		actor.inventory.remove(_item.id);

		if (object* object = actor.region->object_at(actor.coords)) {
			if (item_box* old_box = dynamic_cast<item_box*>(object)) {
				// There's already an item box on the ground at the drop location. Put the item in it.
				old_box->items().push_back(_item.id);
				return cont(result::success);
			} else {
				// There's another object in the way at the drop location.
				return actor.agent().send_message(std::make_unique<message_entity_in_the_way>(), [cont] { return cont(result::aborted); });

				//! @todo It should probably always be possible to drop stuff. This by itself is a good reason to allow multiple objects on a tile.
			}
		} else {
			// Nothing on the ground yet. Make a new box, and put the item in it.
			auto new_box = std::make_unique<item_box>();
			new_box->items().push_back(_item.id);
			actor.region->spawn(std::move(new_box), actor.coords);
			return cont(result::success);
		}
	}

	complete item::toss::perform(being& actor, cont cont)
	{
		//! @todo This.
		return drop{_item}.perform(actor, cont);
	}
}