//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "heal.hpp"

#include "charge_cost.hpp"

#include "entities/beings/being.hpp"
#include "world/coordinates.hpp"

namespace ql::magic {
	void heal::cast(ent caster_id, ent gatestone_id, ent target_id, health healing) {
		// Get target components.
		auto& target_being = reg.get<being>(target_id);
		auto const target_location = reg.get<location>(target_id);

		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		if ((caster_location.coords - target_location.coords).length() > 5_span) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = 1_mp / 1.0_hp / 1.0_hp * healing * healing;
		if (!charge_cost{gatestone, mana_cost}.check_and_pay()) { return; }

		//! @todo Part targeting. For now, just heal everything.
		auto& target_being = reg.get<being>(target_id);
		for (body_part& part : target_being.body.parts()) {
			target_being.heal(healing, part, caster_id);
		}
	}
}
