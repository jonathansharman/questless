/**
* @file    Blood.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Blood particle implementation.
*/

#include "animation/particles/Blood.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	Texture& Blood::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/blood.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
