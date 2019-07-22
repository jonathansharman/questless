//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "teleport.hpp"

#include "charge_cost.hpp"

#include "effects/lightning_bolt.hpp"
#include "entities/beings/being.hpp"
#include "world/region.hpp"

namespace ql::magic {
	void cast(ent caster_id, ent gatestone_id, region_tile::point target) {
		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		auto const distance = (caster_location.coords - target).length();
		if (distance > 15_span) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		if (!charge_cost{gatestone, distance * 5_mp / 1_span}.check_and_pay()) { return; }

		// Add lightning bolt effects to region.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::lightning_bolt(caster_location.coords)});
		region.add_effect({effects::lightning_bolt(target)});

		if (!region.try_move(caster_id, target)) {
			//! @todo Try to move to the nearest free location.
		}
	}
}
