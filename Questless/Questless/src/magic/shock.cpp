//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "shock.hpp"

#include "agents/agent.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "items/magic/gatestone.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"

//! @todo Body part stuff - remove if not used.
#include "entities/beings/body_part.hpp"

namespace ql::magic {
	auto shock::cast(reg& reg, id caster_id, id gatestone_id, tile_hex::point target, dmg::shock damage) -> void {
		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		if ((caster_location.coords - target).length() > 3_pace) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = cancel::quantity_cast<mana>(0.2 * 1_mp / 1_shock / 1_shock * damage * damage);
		if (gatestone.charge < mana_cost) { return; }
		gatestone.charge -= mana_cost;

		//! @todo Shock quality.
		double const quality = 1.0;

		//! Add a lightning bolt effect.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::lightning_bolt{target}});

		if (auto target_entity_id = region.entity_id_at(target)) {
			//! @todo What about damaging objects?
			if (auto target_body = reg.try_get<body>(*target_entity_id)) {
				//! @todo Experimental body part stuff here... Delete or fix.

				// Pick a random strike path from the root to a leaf part and deal progressively lower damage to each part.
				dmg::group group = {cancel::quantity_cast<dmg::shock>(quality * damage)};
				body_part* part = &reg.get<body_part>(target_body->root_part_id);
				for (;;) {
					// Deal damage to the current part.
					part->take_damage(group, caster_id);
					// Reduce the damage by half.
					group /= 2;
					// Stop if there are no attachments.
					if (part->attachments.empty()) { break; }
					// Pick a random attachment.
					auto& attachment = part->attachments[uniform(std::size_t{0}, part->attachments.size() - 1)];
					// Stop if the attachment has no attached part.
					if (!attachment.o_part_id) { break; }
					// Descend the tree.
					part = &reg.get<body_part>(*attachment.o_part_id);
				}
			}
		}
	}
}
