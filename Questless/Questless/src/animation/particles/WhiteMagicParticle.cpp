/**
* @file    WhiteMagicParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION White magic particle implementation.
*/

#include "animation/particles/WhiteMagicParticle.h"
#include "Game.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	void WhiteMagicParticle::subupdate()
	{
		_velocity.step().x *= 1.0 - _vx_percent_drag_rate * Game::frame_duration;
	}

	Texture const& WhiteMagicParticle::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return std::make_unique<Texture>("resources/textures/particles/magic/white.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
