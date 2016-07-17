/**
* @file    EntityAnimator.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the entity animator class.
*/

#include "animation/EntityAnimator.h"

using std::make_unique;

using namespace sdl;

namespace questless
{
	// Beings

	void EntityAnimator::visit(const Human&)
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("ss/human", [] { return Texture::make("resources/textures/human-animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND); });
			first_call = false;
		}

		_animation = make_unique<AnimationCollection>("ss/human", 3, 1);

		vector<Animation::Frame> frames
			{ {double_seconds{0.2}, Point{0, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{2, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			};
		_animation->add("walking", Animation::make(frames, true));
		_animation->start("walking", true);
	}
	void EntityAnimator::visit(const Goblin&)
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("ss/goblin", [] { return Texture::make("resources/textures/test_animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND); });
			first_call = false;
		}

		_animation = make_unique<AnimationCollection>("ss/goblin", 3, 1);

		vector<Animation::Frame> frames
			{ {double_seconds{0.2}, Point{0, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{2, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			};
		_animation->add("walking", Animation::make(frames, true));
		_animation->start("walking", true);
	}

	// Objects

	void EntityAnimator::visit(const ItemBox&)
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("ss/item-box", [] { return Texture::make("resources/textures/entities/objects/item-box.png", sdl::renderer(), SDL_BLENDMODE_BLEND); });
			first_call = false;
		}

		_animation = make_unique<AnimationCollection>("ss/item-box", 1, 1);

		vector<Animation::Frame> frames{{double_seconds{1.0}, Point{0, 0}, Point{0, 4}}};
		_animation->add("being-a-box", Animation::make(frames, true));
		_animation->start("being-a-box", true);
	}
}
