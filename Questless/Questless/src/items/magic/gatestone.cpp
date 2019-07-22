//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "gatestone.hpp"

#include "agents/agent.hpp"
#include "entities/beings/being.hpp"
#include "items/equipment.hpp"
#include "items/item.hpp"
#include "magic/spell.hpp"

namespace ql {
	void make_gatestone(ent id, magic::color color, dynamic_nonnegative<mana> charge, dynamic_nonnegative<tick> cooldown) {
		make_item(id, 1.0_mass);
		make_equipment(id, std::nullopt, {{body_part::tag::hand, std::nullopt}}, 10_tick, 10_tick);
		reg.assign<gatestone>(id, id, color, charge, cooldown);
	}

	void gatestone::update(tick elapsed) {
		cooldown -= elapsed;

		// Charge over time.
		charge += 1_mp / 1_tick * elapsed;
	}
}
