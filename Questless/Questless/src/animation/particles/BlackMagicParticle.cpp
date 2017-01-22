/**
* @file    BlackMagicParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Black magic particle implementation.
*/

#include "animation/particles/BlackMagicParticle.h"
#include "Game.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void BlackMagicParticle::subupdate()
	{
		_velocity *= 1.0 + _acceleration_factor * Game::frame_duration;
		_velocity.step().rotate(GameRadians{_turn_rate * Game::frame_duration});
	}

	Texture const& BlackMagicParticle::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return std::make_unique<Texture>("resources/textures/particles/magic/black.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
