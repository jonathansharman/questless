//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "teleport.hpp"

#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "items/magic/gatestone.hpp"
#include "world/region.hpp"

namespace ql::magic {
	auto teleport::cast(id caster_id, id gatestone_id, tile_hex::point target) -> void {
		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		auto const distance = (caster_location.coords - target).length();
		if (distance > 15_pace) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = distance * 5_mp / 1_pace;
		if (gatestone.charge < mana_cost) { return; }
		gatestone.charge -= mana_cost;

		// Add lightning bolt effects to region.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::lightning_bolt{caster_location.coords}});
		region.add_effect({effects::lightning_bolt{target}});

		if (!region.try_move(caster_id, target)) {
			//! @todo Try to move to the nearest free location.
		}
	}
}
