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

namespace questless
{
	sdl::Texture& BlueMagic::texture() const
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("particles/blue_magic", [] {
				return sdl::Texture::make("resources/textures/particles/magic/blue.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return sdl::texture_manager()["particles/blue_magic"];
	}
}
