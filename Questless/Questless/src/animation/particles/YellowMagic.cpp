/**
* @file    YellowMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the YellowMagic particle class.
*/

#include "animation/particles/YellowMagic.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	void YellowMagic::subupdate()
	{
		_velocity.step().rotate(uniform(-1.0, 1.0) * _max_turn_rate);
		_angle = _velocity.step().angle();
	}

	const Texture& YellowMagic::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/magic/yellow.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
