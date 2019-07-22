//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "equipment.hpp"

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"

namespace ql {
	void make_equipment(ent id, std::optional<ent> o_bearer_id, std::vector<equipment::tab> tabs, tick equip_time, tick unequip_time) {
		reg.assign<equipment>(id, id, o_bearer_id, std::move(tabs), equip_time, unequip_time);
	}

	void equipment::equip(ent actor_id) {
		//! @todo Allow bearer to choose where to equip item.

		for (auto& tab : tabs) {
			bool found = false;
			reg.get<ql::body>(actor_id).for_all_parts([&](body_part& part) {
				if (!found && part.tags.find(tab.tag) != part.tags.end() && !part.equipped_item_id) {
					part.equipped_item_id = id;
					tab.o_part_id = part.id;
					found = true;
				}
			});
			if (!found) { forced_unequip(); }
		}
		o_bearer_id = actor_id;
		reg.get<body>(actor_id).cond.busy_time += equip_time;
	}

	void equipment::unequip() {
		if (!equipped()) return;
		reg.get<body>(*o_bearer_id).cond.busy_time += unequip_time;
		forced_unequip();
	}

	void equipment::forced_unequip() {
		for (auto& tab : tabs) {
			if (tab.o_part_id && reg.valid(*tab.o_part_id)) {
				auto& part = reg.get<body_part>(*tab.o_part_id);
				part.equipped_item_id = std::nullopt;
			}
			tab.o_part_id = std::nullopt;
		}
		o_bearer_id = std::nullopt;
	}
}
