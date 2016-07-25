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

	sdl::Texture& WhiteMagic::texture() const
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("particles/white_magic", [] {
				return sdl::Texture::make("resources/textures/particles/magic/white.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return sdl::texture_manager()["particles/white_magic"];
	}
}