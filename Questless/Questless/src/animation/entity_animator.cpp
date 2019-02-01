//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entity_animator.hpp"

#include "bleeding.hpp"
#include "flame.hpp"
#include "scene_node.hpp"
#include "sprite_animation.hpp"
#include "still_image.hpp"

#include "entities/all_entities.hpp"
#include "rsrc/entity.hpp"

namespace ql {
	entity_animator::entity_animator(rsrc::entity const& resources) : resources{resources} {}

	entity_animator::~entity_animator() = default;

	// Beings

	void entity_animator::visit(human const& human) {
		// Sprite animation
		auto scene_node = umake<ql::scene_node>(umake<ql::sprite_animation>( //
			ql::sprite_sheet{resources.ss.human, {3, 1}},
			std::vector<sprite_animation::frame>{//
				{0.2_s, {0, 0}, {0, 12}},
				{0.2_s, {1, 0}, {0, 12}},
				{0.2_s, {2, 0}, {0, 12}},
				{0.2_s, {1, 0}, {0, 12}}},
			sprite_animation::loop_type::looping,
			sprite_animation::start_time::random));

		// Bleeding animation
		auto total_bleeding = blood_per_tick{0.0};
		for (body_part const& part : human.body.parts()) {
			total_bleeding += part.bleeding.value();
		}
		if (total_bleeding > blood_per_tick{0.0}) {
			// Severity of bleeding is the rate of blood loss over the being's total vitality.
			auto const severity = total_bleeding / human.body.total_vitality();
			// Converts the severity of bleeding to drops of animated blood per second.
			constexpr auto conversion_factor = ql::bleeding::drops{5.0} / 1.0_s / (blood_per_tick{1.0} / 1.0_hp);
			auto bleeding = umake<ql::bleeding>(severity * conversion_factor);
			scene_node->front_children.push_back(std::move(bleeding));
		}

		animation = std::move(scene_node);
	}

	void entity_animator::visit(goblin const& goblin) {
		// Sprite animation
		auto scene_node = umake<ql::scene_node>(umake<ql::sprite_animation>( //
			ql::sprite_sheet{resources.ss.goblin, {3, 1}},
			std::vector<sprite_animation::frame>{//
				{0.2_s, {0, 0}, {0, 12}}, //
				{0.2_s, {1, 0}, {0, 12}}, //
				{0.2_s, {2, 0}, {0, 12}}, //
				{0.2_s, {1, 0}, {0, 12}}},
			sprite_animation::loop_type::looping,
			sprite_animation::start_time::random));

		// Bleeding animation
		auto total_bleeding = blood_per_tick{0.0};
		for (body_part const& part : goblin.body.parts()) {
			total_bleeding += part.bleeding.value();
		}
		if (total_bleeding > blood_per_tick{0.0}) {
			// Severity of bleeding is the rate of blood loss over the being's total vitality.
			auto const severity = total_bleeding / goblin.body.total_vitality();
			// Converts the severity of bleeding to drops of animated blood per second.
			constexpr auto conversion_factor = ql::bleeding::drops{5.0} / 1.0_s / (blood_per_tick{1.0} / 1.0_hp);
			auto bleeding = umake<ql::bleeding>(severity * conversion_factor);
			scene_node->front_children.push_back(std::move(bleeding));
		}

		animation = std::move(scene_node);
	}

	// Objects

	void entity_animator::visit(campfire const&) {
		auto firewood = umake<still_image>(resources.txtr.firewood);
		firewood->set_relative_origin({0.5f, 0.5f});
		auto scene_node = umake<ql::scene_node>(std::move(firewood));

		auto flame = umake<ql::flame>();
		// Pre-update the flame so it's steady immediately.
		flame->update(2.0_s);

		scene_node->front_children.push_back(umake<ql::scene_node>(std::move(flame)));
		animation = std::move(scene_node);
	}

	void entity_animator::visit(corpse const&) {
		animation = umake<still_image>(resources.txtr.grave);
		animation->setOrigin(0, 12);
	}

	void entity_animator::visit(item_box const&) {
		animation = umake<still_image>(resources.txtr.item_box);
		animation->setOrigin(0, 8);
	}
}
