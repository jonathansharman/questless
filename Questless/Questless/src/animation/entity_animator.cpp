//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entity_animator.hpp"

#include "animation_stack.hpp"
#include "bleeding.hpp"
#include "flame.hpp"
#include "sprite_animation.hpp"
#include "still.hpp"

#include "entities/all_entities.hpp"

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	entity_animator::entity_animator() = default;
	entity_animator::~entity_animator() = default;

	// Beings

	void entity_animator::visit(human const& human) {
		auto animation_stack = umake<ql::animation_stack>();

		// Sprite animation
		static auto sprite_sheet = smake<ql::sprite_sheet>
			( the_texture_manager().add("resources/textures/human-animation.png")
			, sprite_sheet_space::vector{3, 1}
			);
		auto sprite_animation = umake<ql::sprite_animation>
			( sprite_sheet
			, std::vector<sprite_animation::frame>
				{ {game_space::seconds{0.2}, sprite_sheet_space::point{0, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 12}}
				}
			, looping{true}
			);
		sprite_animation->reset(randomize_start_time{true});
		animation_stack->add(std::move(sprite_animation));

		// Bleeding animation
		auto total_bleeding = blood_per_tick{0.0};
		for (body_part const& part : human.body.parts()) {
			total_bleeding += part.bleeding.value();
		}
		if (total_bleeding > blood_per_tick{0.0}) {
			constexpr auto bleeding_scaling_factor = 5.0_tick * 1.0_hp / 1.0_blood;
			auto bleeding = umake<ql::bleeding>((total_bleeding / human.body.total_vitality().value() * bleeding_scaling_factor).value);
			animation_stack->add(std::move(bleeding));
		}

		_animation = std::move(animation_stack);
	}
	void entity_animator::visit(goblin const& goblin) {
		auto animation_stack = umake<ql::animation_stack>();

		// Sprite animation
		static auto sprite_sheet = smake<ql::sprite_sheet>
			( the_texture_manager().add("resources/textures/goblin-animation.png")
			, sprite_sheet_space::vector{3, 1}
			);
		auto sprite_animation = umake<ql::sprite_animation>
			( sprite_sheet
			, std::vector<sprite_animation::frame>
				{ {game_space::seconds{0.2}, sprite_sheet_space::point{0, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 12}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 12}}
				}
			, looping{true}
			);
		sprite_animation->reset(randomize_start_time{true});
		animation_stack->add(std::move(sprite_animation));

		// Bleeding animation
		blood_per_tick total_bleeding{0.0};
		for (body_part const& part : goblin.body.parts()) {
			total_bleeding += part.bleeding.value();
		}
		if (total_bleeding > 0.0_blood_per_tick) {
			constexpr double bleeding_scaling_factor = 5.0;
			auto bleeding = umake<ql::bleeding>((total_bleeding / goblin.body.total_vitality().value() / body_part::blood_per_vitality * bleeding_scaling_factor).value);
			animation_stack->add(std::move(bleeding));
		}

		_animation = std::move(animation_stack);
	}

	// Objects
	
	void entity_animator::visit(campfire const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/campfire.png");
		auto animation_stack = umake<ql::animation_stack>();
		animation_stack->add(umake<still>(texture_handle, texture_space::vector{0, 0}));
		auto flame = umake<ql::flame>();
		// Pre-update the flame a number of times so it's steady immediately.
		constexpr int n_pre_updates = 100;
		for (int i = 0; i < n_pre_updates; ++i) {
			flame->update();
		}
		animation_stack->add(std::move(flame));
		_animation = std::move(animation_stack);
	}
	void entity_animator::visit(corpse const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/grave.png");
		_animation = umake<still>(texture_handle, texture_space::vector{0, 12});
	}
	void entity_animator::visit(item_box const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/item-box.png");
		_animation = umake<still>(texture_handle, texture_space::vector{0, 8});
	}
}
