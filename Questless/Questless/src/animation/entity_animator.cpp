//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/entity_animator.hpp"

#include "animation/animation_stack.hpp"
#include "animation/flame.hpp"
#include "animation/sprite_animation.hpp"
#include "animation/still.hpp"
#include "entities/all_entities.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	// Beings

	void entity_animator::visit(human const&)
	{
		static auto sprite_sheet = std::make_shared<ql::sprite_sheet>
			( the_texture_manager().add("resources/textures/human-animation.png")
			, sprite_sheet_space::vector{3, 1}
			);
		auto sprite_animation = std::make_unique<ql::sprite_animation>
			( sprite_sheet
			, std::vector<sprite_animation::frame>
				{ {game_space::seconds{0.2}, sprite_sheet_space::point{0, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 6}}
				}
			, looping{true}
			);
		sprite_animation->reset(randomize_start_time{true});
		_animation = std::move(sprite_animation);
	}
	void entity_animator::visit(goblin const&)
	{
		static auto sprite_sheet = std::make_shared<ql::sprite_sheet>
			( the_texture_manager().add("resources/textures/goblin-animation.png")
			, sprite_sheet_space::vector{3, 1}
			);
		auto sprite_animation = std::make_unique<ql::sprite_animation>
			( sprite_sheet
			, std::vector<sprite_animation::frame>
				{ {game_space::seconds{0.2}, sprite_sheet_space::point{0, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 6}}
				, {game_space::seconds{0.2}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 6}}
				}
			, looping{true}
			);
		sprite_animation->reset(randomize_start_time{true});
		_animation = std::move(sprite_animation);
	}

	// Objects
	
	void entity_animator::visit(campfire const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/campfire.png");
		auto animation_stack = std::make_unique<ql::animation_stack>();
		animation_stack->add(std::make_unique<still>(texture_handle, texture_space::vector{0, 0}));
		animation_stack->add(std::make_unique<flame>());
		_animation = std::move(animation_stack);
	}
	void entity_animator::visit(corpse const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/grave.png");
		_animation = std::make_unique<still>(texture_handle, texture_space::vector{0, 6});
	}
	void entity_animator::visit(item_box const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/entities/objects/item-box.png");
		_animation = std::make_unique<still>(texture_handle, texture_space::vector{0, 4});
	}
}
