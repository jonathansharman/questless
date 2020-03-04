//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "body_part.hpp"

#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "world/region.hpp"

#include <range/v3/view/transform.hpp>

namespace ql {
	namespace {
		auto generate_attached_parts_helper(reg& reg, id part_id) -> void {
			auto const& part = reg.get<body_part>(part_id);
			auto const n = part.attachments.size();
			auto const owner_id = part.owner_id;
			for (size_t i = 0; i < n; ++i) {
				auto const attached_part_id = reg.get<body_part>(part_id).attachments[i].default_generator.make(reg, owner_id);
				generate_attached_parts_helper(reg, attached_part_id);
				reg.get<body_part>(part_id).attachments[i].o_part_id = attached_part_id;
			}
		}
	}

	body_part::body_part(ql::reg& reg, ql::id id, ql::id owner_id) : reg{&reg}, id{id}, owner_id{owner_id} {}

	auto body_part::update(tick elapsed) -> void {
		body& owner = reg->get<body>(owner_id);

		{ // Handle temperature damage.
			auto const& location = reg->get<ql::location>(owner_id);
			auto const& region = reg->get<ql::region>(location.region_id);
			auto const temp = region.temperature(location.coords);
			if (temp > stats.max_temp.cur) {
				constexpr auto temperature_scorch_rate = 1_scorch / 1_temp / 1_tick;
				dmg::group scorch{(temp - stats.max_temp.cur) * temperature_scorch_rate * elapsed};
				take_damage(scorch, std::nullopt);
			} else if (temp < stats.min_temp.cur) {
				constexpr auto temperature_freeze_rate = 1_freeze / 1_temp / 1_tick;
				dmg::group freeze{(stats.min_temp.cur - temp) * temperature_freeze_rate * elapsed};
				take_damage(freeze, std::nullopt);
			}
		}

		{ // Heal wounds.
			if (owner.cond.starving()) {
				// Don't regenerate if starving.
				stats.regen_factor.cur = 0;
			} else {
				stats.regen_factor.reset();
				if (owner.cond.asleep()) {
					// Regenerate twice as fast while sleeping.
					stats.regen_factor.cur *= 2;
				}
				if (owner.cond.hungry()) {
					// Regenerate half as fast while hungry
					stats.regen_factor.cur /= 2;
				}
			}
		}
	}

	auto body_part::take_damage(dmg::group& damage, std::optional<ql::id> o_source_id) -> void {
		// Apply part's equipped item's armor.
		if (equipped_item_id) {
			if (auto armor = reg->try_get<dmg::armor>(*equipped_item_id)) { damage = damage.against(*armor); }
		}
		// Apply part's armor.
		damage = damage.against(stats.armor.cur);
		// Apply owner's armor.
		auto const& body = reg->get<ql::body>(owner_id);
		damage = damage.against(body.stats.armor.cur);

		// Apply secondary damage effects.
		//! @todo Add effects for other damage types. Balance numbers.
		for (dmg::damage const& damage_part : damage.parts) {
			match(
				damage_part,
				[&](dmg::slash const& s) { status_set.wounds.push_back(wound{s * 1_laceration / 1_slash}); },
				[&](dmg::pierce const& p) { status_set.wounds.push_back(wound{p * 1_puncture / 1_pierce}); },
				[&](dmg::cleave const& c) {
					status_set.wounds.push_back(wound{c * 1_laceration / 2_cleave});
					status_set.wounds.push_back(wound{c * 1_puncture / 2_cleave});
				},
				[&](dmg::bludgeon const& b) {
					status_set.wounds.push_back(wound{b * 1_bruise / 2_bludgeon});
					status_set.wounds.push_back(wound{b * 1_fracture / 2_bludgeon});
				},
				[&](dmg::scorch const& s) {
					status_set.wounds.push_back(wound{s * 1_burn / 1_scorch});
					//! @todo Cauterize lacerations here?
				},
				[&](dmg::freeze const& f) { status_set.wounds.push_back(wound{f * 1_frostbite / 1_freeze}); },
				[&](dmg::shock const& s) { status_set.wounds.push_back(wound{s * 1_burn / 1_shock}); },
				[&](dmg::poison const&) {},
				[&](dmg::rot const&) {});
		}

		// Add injury effect.
		auto const location = reg->get<ql::location>(owner_id);
		reg->get<region>(location.region_id).add_effect({effects::injury{location.coords, damage, owner_id, id, o_source_id}});
	}

	auto body_part::generate_attached_parts() -> void {
		generate_attached_parts_helper(*reg, id);
	}
}
