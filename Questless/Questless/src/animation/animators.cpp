//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animators.hpp"

#include "animation.hpp"
#include "bleeding.hpp"
#include "flame.hpp"
#include "scene_node.hpp"
#include "still_image.hpp"

#include "entities/all_entities.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/body_part.hpp"
#include "entities/entity.hpp"
#include "entities/objects/object.hpp"
#include "magic/spell.hpp"
#include "rsrc/entity.hpp"
#include "rsrc/particle.hpp"
#include "rsrc/spell.hpp"
#include "rsrc/tile.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/tile.hpp"

namespace ql {
	uptr<animation> animate(rsrc::tile const& resources, tile const& tile) {
		return match(tile.terrain.value,
			[&](dirt const&) { return umake<still_image>(resources.txtr.dirt); },
			[&](edge const&) {
				//! @todo Edge tiles should simply not be drawn.
				return umake<still_image>(resources.txtr.blank);
			},
			[&](grass const&) { return umake<still_image>(resources.txtr.grass); },
			[&](sand const&) { return umake<still_image>(resources.txtr.sand); },
			[&](snow const&) { return umake<still_image>(resources.txtr.snow); },
			[&](stone const&) { return umake<still_image>(resources.txtr.stone); },
			[&](water const&) { return umake<still_image>(resources.txtr.water); });
	}

	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell) {
		return match(spell.value,
			[&](magic::telescope const&) { return umake<still_image>(resources.txtr.telescope); },
			[&](magic::heal const&) { return umake<still_image>(resources.txtr.heal); },
			[&](magic::shock const&) { return umake<still_image>(resources.txtr.shock); },
			[&](magic::teleport const&) { return umake<still_image>(resources.txtr.teleport); });
	}

	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, entity const& entity) {
		return match(entity.value,
			[&](being const& being) { return animate(entity_resources, particle_resources, being); },
			[&](object const& object) { return animate(entity_resources, particle_resources, object); });
	}

	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, being const& being) {
		return match(being.species.value,
			[&](human const&) {
				// Sprite animation
				auto scene_node = umake<ql::scene_node>(umake<ql::sprite_animation>( //
					ql::sprite_sheet{entity_resources.ss.human, {3, 1}},
					std::vector<sprite_animation::frame>{//
						{0.2_s, {0, 0}, {0, 12}},
						{0.2_s, {1, 0}, {0, 12}},
						{0.2_s, {2, 0}, {0, 12}},
						{0.2_s, {1, 0}, {0, 12}}},
					sprite_animation::loop_type::looping,
					sprite_animation::start_time::random));

				// Bleeding animation
				auto total_bleeding = blood_per_tick{0.0};
				for (body_part const& part : being.body.parts()) {
					total_bleeding += part.bleeding.value();
				}
				if (total_bleeding > blood_per_tick{0.0}) {
					// Severity of bleeding is the rate of blood loss over the being's total vitality.
					auto const severity = total_bleeding / being.body.total_vitality();
					// Converts the severity of bleeding to drops of animated blood per second.
					constexpr auto conversion_factor = ql::bleeding::drops{5.0} / 1.0_s / (blood_per_tick{1.0} / 1.0_hp);
					auto bleeding = umake<ql::bleeding>(particle_resources, severity * conversion_factor);
					scene_node->front_children.push_back(std::move(bleeding));
				}
				return scene_node;
			},
			[&](goblin const&) {
				// Sprite animation
				auto scene_node = umake<ql::scene_node>(umake<ql::sprite_animation>( //
					ql::sprite_sheet{entity_resources.ss.goblin, {3, 1}},
					std::vector<sprite_animation::frame>{//
						{0.2_s, {0, 0}, {0, 12}}, //
						{0.2_s, {1, 0}, {0, 12}}, //
						{0.2_s, {2, 0}, {0, 12}}, //
						{0.2_s, {1, 0}, {0, 12}}},
					sprite_animation::loop_type::looping,
					sprite_animation::start_time::random));

				// Bleeding animation
				auto total_bleeding = blood_per_tick{0.0};
				for (body_part const& part : being.body.parts()) {
					total_bleeding += part.bleeding.value();
				}
				if (total_bleeding > blood_per_tick{0.0}) {
					// Severity of bleeding is the rate of blood loss over the being's total vitality.
					auto const severity = total_bleeding / being.body.total_vitality();
					// Converts the severity of bleeding to drops of animated blood per second.
					constexpr auto conversion_factor = ql::bleeding::drops{5.0} / 1.0_s / (blood_per_tick{1.0} / 1.0_hp);
					auto bleeding = umake<ql::bleeding>(particle_resources, severity * conversion_factor);
					scene_node->front_children.push_back(std::move(bleeding));
				}
				return scene_node;
			});
	}

	uptr<animation> animate(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, object const& object) {
		return match(object.subtype,
			[&](campfire const&) -> uptr<animation> {
				auto firewood = umake<still_image>(entity_resources.txtr.firewood);
				firewood->set_relative_origin({0.5f, 0.5f});
				auto scene_node = umake<ql::scene_node>(std::move(firewood));

				auto flame = umake<ql::flame>(particle_resources);
				// Pre-update the flame so it's steady immediately.
				flame->update(2.0_s);

				scene_node->front_children.push_back(umake<ql::scene_node>(std::move(flame)));
				return scene_node;
			},
			[&](corpse const&) -> uptr<animation> {
				auto result = umake<still_image>(entity_resources.txtr.grave);
				result->setOrigin(0, 12);
				return result;
			},
			[&](item_box const&) -> uptr<animation> {
				auto result = umake<still_image>(entity_resources.txtr.item_box);
				result->setOrigin(0, 8);
				return result;
			});
	}
}
