//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "telescope.hpp"

#include "effects/telescope.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/body_status.hpp"
#include "items/magic/gatestone.hpp"
#include "world/region.hpp"

namespace ql::magic {
	void telescope::cast(ent caster_id, ent gatestone_id) {
		//! @todo Pick duration and bonus and compute cost accordingly.

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = 10_mp;
		if (gatestone.charge < 10_mp) { return; }
		gatestone.charge -= mana_cost;

		// Add status to caster.
		auto& caster_body = reg.get<body>(caster_id);
		caster_body.status_set.timed.push_back({50_tick, {telescoped{100_perception, caster_id}}});

		// Add telescope effect to region.
		auto const caster_location = reg.get<location>(caster_id);
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::telescope{caster_location.coords, caster_id}});
	}
}
