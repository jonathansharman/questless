/**
* @file    RedMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the RedMagic particle class.
*/

#include "animation/particles/RedMagic.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	void RedMagic::subupdate()
	{
		_velocity.step().y += _gravity * frame_duration;
	}

	Texture& RedMagic::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/magic/red.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
