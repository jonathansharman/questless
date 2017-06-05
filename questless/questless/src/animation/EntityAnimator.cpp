//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/EntityAnimator.h"

#include "animation/AnimationStack.h"
#include "animation/Flame.h"
#include "animation/SpriteAnimation.h"
#include "animation/Still.h"
#include "entities/all-entities.h"

using namespace sdl;
using namespace units;

namespace questless
{
	// Beings

	void EntityAnimator::visit(Human const&)
	{
		static auto sprite_sheet = std::make_shared<SpriteSheet>
			(texture_manager().add("resources/textures/human-animation.png")
				, SpriteSheetSpace::Vector{3, 1}
		);
		auto sprite_animation = std::make_unique<SpriteAnimation>
			( sprite_sheet
			, std::vector<SpriteAnimation::Frame>
				{ {GameSeconds{0.2}, SpriteSheetSpace::Point{0, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{1, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{2, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{1, 0}, TextureSpace::Point{0, 10}}
				}
			, Looping{true}
			);
		sprite_animation->reset(RandomizeStartTime{true});
		_animation = std::move(sprite_animation);
	}
	void EntityAnimator::visit(Goblin const&)
	{
		static auto sprite_sheet = std::make_shared<SpriteSheet>
			( texture_manager().add("resources/textures/goblin-animation.png")
			, SpriteSheetSpace::Vector{3, 1}
			);
		auto sprite_animation = std::make_unique<SpriteAnimation>
			( sprite_sheet
			, std::vector<SpriteAnimation::Frame>
				{ {GameSeconds{0.2}, SpriteSheetSpace::Point{0, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{1, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{2, 0}, TextureSpace::Point{0, 10}}
				, {GameSeconds{0.2}, SpriteSheetSpace::Point{1, 0}, TextureSpace::Point{0, 10}}
				}
			, Looping{true}
			);
		sprite_animation->reset(RandomizeStartTime{true});
		_animation = std::move(sprite_animation);
	}

	// Objects
	
	void EntityAnimator::visit(Campfire const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/entities/objects/campfire.png");
		auto animation_stack = std::make_unique<AnimationStack>();
		animation_stack->add(std::make_unique<Still>(texture_handle, TextureSpace::Point{0, 0}));
		animation_stack->add(std::make_unique<Flame>());
		_animation = std::move(animation_stack);
	}
	void EntityAnimator::visit(Corpse const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/entities/objects/grave.png");
		_animation = std::make_unique<Still>(texture_handle, TextureSpace::Point{0, 10});
	}
	void EntityAnimator::visit(ItemBox const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/entities/objects/item-box.png");
		_animation = std::make_unique<Still>(texture_handle, TextureSpace::Point{0, 4});
	}
}
