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
	sdl::Texture::ptr& BlueMagic::texture() const
	{
		static auto texture = sdl::Texture::make("resources/textures/particles/magic/blue.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
