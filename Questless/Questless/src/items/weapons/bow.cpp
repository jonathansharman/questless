//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/bow.hpp"
#include "items/weapons/arrow.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql
{
	//! @todo These functions are O(n). Should be easy to find an efficient O(1) solution.

	complete bow::shoot::arrow_cost::check(being& actor, cont cont) const
	{
		for (item& maybe_quiver : actor.inventory.items) {
			if (quiver* quiver = dynamic_cast<ql::quiver*>(&maybe_quiver)) {
				if (quiver->equipped() && quiver->opt_bearer_id() == actor.id) {
					for (item& maybe_arrow : quiver->inventory.items) {
						if (dynamic_cast<arrow*>(&maybe_arrow)) {
							// Found an arrow in an equipped quiver.
							return cont();
						}
					}
				}
			}
		}
		for (item& item : actor.inventory.items) {
			if (dynamic_cast<arrow*>(&item)) {
				// Found an arrow free-floating in the inventory.
				return cont();
			}
		}
		return actor.agent().send_message(std::make_unique<message_not_enough_ammo>(), [cont] { return complete{}; });
	}

	void bow::shoot::arrow_cost::incur(being& actor) const
	{
		for (item& maybe_quiver : actor.inventory.items) {
			if (quiver* quiver = dynamic_cast<ql::quiver*>(&maybe_quiver)) {
				if (quiver->equipped() && quiver->opt_bearer_id() == actor.id) {
					for (item& maybe_arrow : quiver->inventory.items) {
						if (arrow* arrow = dynamic_cast<ql::arrow*>(&maybe_arrow)) {
							// Found an arrow in an equipped quiver.
							quiver->inventory.remove(arrow->id);
							return;
						}
					}
				}
			}
		}
		for (item& item : actor.inventory.items) {
			if (dynamic_cast<arrow*>(&item)) {
				// Found an arrow free-floating in the inventory.
				actor.inventory.remove(item.id);
			}
		}
		throw std::logic_error{"Couldn't find arrow to spend."};
	}
}
