/**
* @file    WhiteMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the WhiteMagic particle class.
*/

#include "animation/particles/WhiteMagic.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	void WhiteMagic::subupdate()
	{
		_velocity.y += Hertz{_gravity * frame_duration};
		_velocity.x *= _vx_factor_per_frame;
	}

	sdl::Texture::ptr& WhiteMagic::texture() const
	{
		static auto texture = sdl::Texture::make("resources/textures/particles/magic/white.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}