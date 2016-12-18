/**
* @file    BlueMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BlueMagic particle class.
*/

#include "animation/particles/BlueMagic.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	const Texture& BlueMagic::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/magic/blue.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
