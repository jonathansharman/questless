//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "telescope.hpp"

#include "charge_cost.hpp"
#include "effects/telescope.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/statuses/telescoped.hpp"
#include "world/region.hpp"

namespace ql::magic {
	action::result telescope::cast::perform(being& caster) {
		// Get gatestone.
		if (!reg.valid(gatestone_id)) { return result::failure; }
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		if (!charge_cost{gatestone, 10.0_mp}.check_and_pay()) { return result::failure; }

		// Add status to caster.
		caster.add_status(umake<telescoped>(caster.stats.a.vision.acuity, 50_tick, caster.id)); //! @todo Get duration.
		auto const caster_location = reg.get<location>(caster_id);

		// Add telescope effect to region.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect(effects::telescope{caster.coords, caster.id});

		return action::result::success;
	}
}
