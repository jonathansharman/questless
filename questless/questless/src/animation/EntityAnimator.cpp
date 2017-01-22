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

using std::make_unique;

using namespace sdl;

namespace questless
{
	// Beings

	void EntityAnimator::visit(Human const&)
	{
		static bool first_call = true;
		static Handle<Texture> ss_handle;
		if (first_call) {
			ss_handle = sdl::texture_manager().add([] {
				return make_unique<Texture>("resources/textures/human-animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}

		_animation_set = make_unique<AnimationSet>(ss_handle, 3, 1);

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
		static bool first_call = true;
		static Handle<Texture> ss_handle;
		if (first_call) {
			ss_handle = sdl::texture_manager().add([] {
				return make_unique<Texture>("resources/textures/test-animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}

		_animation_set = make_unique<AnimationSet>(ss_handle, 3, 1);

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
		static bool first_call = true;
		static Handle<Texture> ss_handle;
		if (first_call) {
			ss_handle = sdl::texture_manager().add([] {
				return make_unique<Texture>("resources/textures/entities/objects/grave.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}

		_animation_set = make_unique<AnimationSet>(ss_handle, 1, 1);

		auto animation = _animation_set->add(Animation
			{ {{GameSeconds{1.0}, SpriteSheetPoint{0, 0}, TexturePoint{0, 10}}}
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}
	void EntityAnimator::visit(ItemBox const&)
	{
		static bool first_call = true;
		static Handle<Texture> ss_handle;
		if (first_call) {
			ss_handle = sdl::texture_manager().add([] {
				return make_unique<Texture>("resources/textures/entities/objects/item-box.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}

		_animation_set = make_unique<AnimationSet>(ss_handle, 1, 1);

		auto animation = _animation_set->add(Animation
			{ {{GameSeconds{1.0}, SpriteSheetPoint{0, 0}, TexturePoint{0, 4}}}
			, Looping{true}
			});
		_animation_set->start(animation, RandomizeStartTime{true});
	}
}
