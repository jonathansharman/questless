//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "body_part.hpp"

#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "world/region.hpp"

namespace ql {
	void body_part::update(tick elapsed) {
		body& owner = reg.get<body>(owner_id);

		{ // Handle temperature damage.
			auto const& location = reg.get<ql::location>(owner_id);
			auto const& region = reg.get<ql::region>(location.region_id);
			auto const temp = region.temperature(location.coords);
			if (temp > stats.max_temp) {
				constexpr auto temperature_scorch_rate = 1_scorch / 1.0_temp / 1_tick;
				dmg::group scorch = (temp - stats.max_temp) * temperature_scorch_rate * elapsed;
				take_damage(scorch, std::nullopt);
			} else if (temp < stats.min_temp) {
				constexpr auto temperature_freeze_rate = 1_freeze / 1.0_temp / 1_tick;
				dmg::group freeze = (stats.min_temp - temp) * temperature_freeze_rate * elapsed;
				take_damage(freeze, std::nullopt);
			}
		}

		{ // Heal wounds.
			if (owner.cond.starving()) {
				// Don't regenerate if starving.
				stats.regen_factor = 0;
			} else {
				stats.regen_factor = base_stats.regen_factor;
				if (owner.cond.asleep()) {
					// Regenerate twice as fast while sleeping.
					stats.regen_factor *= 2;
				}
				if (owner.cond.hungry()) {
					// Regenerate half as fast while hungry
					stats.regen_factor /= 2;
				}
			}
		}
	}

	void body_part::take_damage(dmg::group& damage, std::optional<ent> o_source_id) {
		// Apply part's equipped item's armor.
		if (equipped_item_id) {
			if (auto armor = reg.try_get<dmg::armor>(*equipped_item_id)) { damage = damage.against(*armor); }
		}
		// Apply part's armor.
		damage = damage.against(stats.armor);
		// Apply owner's armor.
		auto const& body = reg.get<ql::body>(owner_id);
		damage = damage.against(body.stats.armor);

		// Apply secondary damage effects.
		//! @todo Add effects for other damage types. Balance numbers.
		for (dmg::damage const& damage_part : damage.parts) {
			match(
				damage_part,
				[&](dmg::slash const& slash) { status_set.wounds.emplace_back(slash * 1_laceration / 1_slash); },
				[&](dmg::pierce const& pierce) { status_set.wounds.emplace_back(pierce * 1_puncture / 1_slash); },
				[&](dmg::cleave const& cleave) {
					status_set.wounds.emplace_back(cleave * 1_laceration / 2_cleave);
					status_set.wounds.emplace_back(cleave * 1_puncture / 2_cleave);
				},
				[&](dmg::bludgeon const& bludgeon) {
					status_set.wounds.emplace_back(bludgeon * 1_bruise / 2_bludgeon);
					status_set.wounds.emplace_back(bludgeon * 1_fracture / 2_bludgeon);
				},
				[&](dmg::scorch const& scorch) {
					status_set.wounds.emplace_back(scorch * 1_burn / 1_scorch);
					//! @todo Cauterize lacerations here?
				},
				[&](dmg::freeze const& freeze) { status_set.wounds.emplace_back(freeze * 1_frostbite / 1_freeze); },
				[&](dmg::shock const& shock) { status_set.wounds.emplace_back(shock * 1_burn / 1_shock); },
				[&](dmg::poison const&) {},
				[&](dmg::rot const&) {});
		};

		// Add injury effect.
		auto const location = reg.get<ql::location>(owner_id);
		reg.get<region>(location.region_id).add_effect({effects::injury{location.coords, damage, owner_id, id, o_source_id}});
	}

	void body_part::generate_attached_parts() {
		for (auto& attachment : attachments) {
			ent const part_id = attachment.make_default();
			attachment.o_part_id = part_id;
			reg.get<body_part>(part_id).generate_attached_parts();
		}
	}
}
