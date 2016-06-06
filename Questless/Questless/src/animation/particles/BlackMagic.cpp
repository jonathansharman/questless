/**
* @file    BlackMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BlackMagic particle class.
*/

#include "animation/particles/BlackMagic.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	void BlackMagic::subupdate()
	{
		_velocity += _velocity * (_acceleration_factor * frame_duration);
		_velocity.rotate(_turn_rate * frame_duration);
	}

	sdl::Texture::ptr& BlackMagic::texture() const
	{
		static auto texture = sdl::Texture::make("resources/textures/particles/magic/black.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
