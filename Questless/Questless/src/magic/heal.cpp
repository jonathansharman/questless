//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "heal.hpp"

#include "charge_cost.hpp"
#include "entities/beings/being.hpp"

namespace ql::magic {
	action::result heal::cast::perform(being& caster) {
		// Get target components.
		if (!reg.valid(target_id)) { return result::failure; }
		auto& target_being = reg.get<being>(target_id);
		auto const target_location = reg.get<location>(target_id);

		// Check range.
		auto const caster_location = reg.get<location>(caster.id);
		if ((caster_location.coords - target_location.coords).length() > 5_span) { return result::failure; }

		// Get gatestone.
		if (!reg.valid(gatestone_id)) { return result::failure; }
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		constexpr auto cost_factor = 1.0_mp / 1.0_hp / 1.0_hp;
		if (!charge_cost{gatestone, cost_factor * healing * healing}.check_and_pay()) { return result::failure; }

		//! @todo Part targeting. For now, just heal everything.
		auto& target_being = reg.get<being>(target_id);
		for (body_part& part : target_being.body.parts()) {
			target_being.heal(healing, part, caster_id);
		}

		return result::success;
	}
}
