//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

namespace ql {
	action::result equip::perform(being& actor) {
		// Get equipment.
		if (!reg.valid(equipment_id)) { return result::failure; }
		auto& equipment = reg.get<ql::equipment>(equipment_id);

		//! @todo Allow bearer to choose where to equip item.

		for (auto& tab : equipment.tabs) {
			bool found = false;
			for (body_part& part : actor.body.parts()) {
				if (part.tags.find(tab.tag) != part.tags.end() && !part.equipped_item_id) {
					part.equipped_item_id = equipment.id;
					tab.opt_part_id = part.id;
					found = true;
					break;
				}
			}
			if (!found) {
				equipment.unequip();
				return result::failure;
			}
		}
		equipment.opt_bearer_id = actor.id;
		actor.cond.busy_time += equipment.equip_time;

		return result::success;
	}

	action::result unequip::perform(being& actor) {
		// Get equipment.
		if (!reg.valid(equipment_id)) { return result::failure; }
		auto& equipment = reg.get<ql::equipment>(equipment_id);

		actor.cond.busy_time += equipment.unequip_time;
		equipment.unequip();

		return result::success;
	}

	void equipment::unequip() {
		for (auto& tab : tabs) {
			if (tab.opt_part_id && reg.valid(*tab.opt_part_id)) {
				auto& part = reg.get<body_part>(*tab.opt_part_id);
				part.equipped_item_id = std::nullopt;
			}
			tab.opt_part_id = std::nullopt;
		}
		opt_bearer_id = std::nullopt;
	}
}
