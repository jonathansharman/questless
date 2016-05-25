/**
* @file    EntityAnimator.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the entity animator class, which generates animations for entities.
*/

#ifndef ENTITY_ANIMATOR_H
#define ENTITY_ANIMATOR_H

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include "entities/EntityVisitor.h"
#include "AnimationCollection.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	class EntityAnimator : public EntityVisitor
	{
	public:
		void visit(const Human&) override
		{
			static bool first_call = true;
			if (first_call) {
				sdl::texture_manager().add("ss/human", [] { return Texture::make("resources/textures/human-animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND); });
				first_call = false;
			}

			_animation = make_unique<AnimationCollection>(sdl::texture_manager()["ss/human"], 3, 1);

			vector<Animation::Frame> frames
			{{double_seconds{0.2}, Point{0, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{2, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			};
			_animation->add("walking", Animation{frames, true});
			_animation->start("walking", true);
		}
		void visit(const Goblin&) override
		{
			static bool first_call = true;
			if (first_call) {
				sdl::texture_manager().add("ss/goblin", [] { return Texture::make("resources/textures/test_animation.png", sdl::renderer(), SDL_BLENDMODE_BLEND); });
				first_call = false;
			}

			_animation = make_unique<AnimationCollection>(sdl::texture_manager()["ss/goblin"], 3, 1);

			vector<Animation::Frame> frames
			{{double_seconds{0.2}, Point{0, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{2, 0}, Point{0, 10}}
			, {double_seconds{0.2}, Point{1, 0}, Point{0, 10}}
			};
			_animation->add("walking", Animation{frames, true});
			_animation->start("walking", true);
		}

		unique_ptr<AnimationCollection> animation() { return std::move(_animation); }
	private:
		unique_ptr<AnimationCollection> _animation;
	};
}

#endif