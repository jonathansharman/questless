//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "heal.hpp"

#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"
#include "items/magic/gatestone.hpp"
#include "world/coordinates.hpp"

namespace ql::magic {
	auto heal::cast(id caster_id, id gatestone_id, id target_id, health healing) -> void {
		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		auto const target_location = reg.get<location>(target_id);
		if ((caster_location.coords - target_location.coords).length() > 5_pace) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = 1_mp / 1_hp / 1_hp * healing * healing;
		if (gatestone.charge < mana_cost) { return; }
		gatestone.charge -= mana_cost;

		//! @todo Part targeting, wound type, amount, etc. For now, just heal all wounds.
		reg.get<body>(target_id).for_all_parts([](body_part& part) { part.status_set.wounds.clear(); });
	}
}
