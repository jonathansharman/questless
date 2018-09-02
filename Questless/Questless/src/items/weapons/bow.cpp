//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/bow.hpp"


#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/quiver.hpp"
#include "units/hex_space.hpp"

using namespace units;

namespace ql {
	std::vector<uptr<action>> bow::actions() {
		std::vector<uptr<action>> actions;
		if (equipped()) {
			actions.push_back(_shoot->launch());
			actions.push_back(unequip::make(*this));
		} else {
			actions.push_back(equip::make(*this));
			actions.push_back(drop::make(*this));
			actions.push_back(toss::make(*this));
		}
		return actions;
	}

	//! @todo These functions are O(n). Should be easy to find an efficient O(1) solution.

	complete bow::shoot::arrow_cost::check(being& actor, cont cont) const {
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
		return actor.agent().send_message(umake<message_not_enough_ammo>(), [] { return complete{}; });
	}

	void bow::shoot::arrow_cost::incur(being& actor) const {
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
		assert(false && "Couldn't find arrow to spend.");
	}
	
	sptr<effect> bow::shoot::get_effect(region_tile::point source, region_tile::point target) {
		return smake<arrow_attack_effect>(source, target);
	}
}
