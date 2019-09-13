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
	auto make_gatestone( //
		id gatestone_id,
		magic::color color,
		dynamic_nonnegative<mana> charge,
		dynamic_nonnegative<tick> cooldown) -> id //
	{
		make_item(gatestone_id, 1.0_mass);
		make_equipment(gatestone_id, std::nullopt, {{body_part::tag::hand, std::nullopt}}, 10_ap, 10_ap);
		reg.assign<gatestone>(gatestone_id, gatestone_id, color, charge, cooldown);
		return gatestone_id;
	}

	auto gatestone::update(tick elapsed) -> void {
		cooldown -= elapsed;

		// Charge over time.
		charge += 1_mp / 1_tick * elapsed;
	}
}
