/**
* @file    EntityAnimator.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the entity animator class.
*/

#include "animation/EntityAnimator.h"

#include "units/Point.h"

using namespace units;
using namespace sdl;

namespace questless
{
	// Beings

	void EntityAnimator::visit(Human const&)
	{
		static Handle<Texture> ss_handle = texture_manager().add("resources/textures/human-animation.png");
		_animation_set = std::make_unique<AnimationSet>(ss_handle, 3, 1);

		auto animation = _animation_set->add(Animation
			{ { {GameSeconds{0.2}, SpriteSheetPoint{0, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{1, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{2, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{1, 0}, TexturePoint{0, 10}}
			  }
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}
	void EntityAnimator::visit(Goblin const&)
	{
		static Handle<Texture> ss_handle = texture_manager().add("resources/textures/test-animation.png");
		_animation_set = std::make_unique<AnimationSet>(ss_handle, 3, 1);

		auto animation = _animation_set->add(Animation
			{ { {GameSeconds{0.2}, SpriteSheetPoint{0, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{1, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{2, 0}, TexturePoint{0, 10}}
			  , {GameSeconds{0.2}, SpriteSheetPoint{1, 0}, TexturePoint{0, 10}}
			  }
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}

	// Objects

	void EntityAnimator::visit(Corpse const&)
	{
		static Handle<Texture> ss_handle = texture_manager().add("resources/textures/entities/objects/grave.png");
		_animation_set = std::make_unique<AnimationSet>(ss_handle, 1, 1);

		auto animation = _animation_set->add(Animation
			{ {{GameSeconds{1.0}, SpriteSheetPoint{0, 0}, TexturePoint{0, 10}}}
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}
	void EntityAnimator::visit(ItemBox const&)
	{
		static Handle<Texture> ss_handle = texture_manager().add("resources/textures/entities/objects/item-box.png");
		_animation_set = std::make_unique<AnimationSet>(ss_handle, 1, 1);

		auto animation = _animation_set->add(Animation
			{ {{GameSeconds{1.0}, SpriteSheetPoint{0, 0}, TexturePoint{0, 4}}}
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}
}
