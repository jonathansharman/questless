//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "teleport.hpp"

#include "charge_cost.hpp"

#include "effects/lightning_bolt.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "world/region.hpp"

namespace ql::magic {
	action::result teleport::cast::perform(being& caster) {
		// Check range.
		auto const caster_location = reg.get<location>(caster.id);
		auto const distance = (caster_location.coords - target).length();
		if (distance > 15_span) { return action::result::failure; }

		// Get gatestone.
		if (!reg.valid(gatestone_id)) { return action::result::failure; }
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);

		// Check and pay cost.
		if (!charge_cost{gatestone, distance * 5.0_mp / 1_span}.check_and_pay()) { return action::result::failure; }

		// Add lightning bolt effects to region.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::lightning_bolt(caster_location.coords)});
		region.add_effect({effects::lightning_bolt(target)});

		if (!region.try_move(caster.id, target)) {
			//! @todo Try to move to the nearest free location.
		}

		return action::result::success;
	}
}
