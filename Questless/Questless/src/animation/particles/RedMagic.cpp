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

namespace questless
{
	void RedMagic::subupdate()
	{
		_velocity.y += Hertz{_gravity * frame_duration};
	}

	sdl::Texture& RedMagic::texture() const
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("particles/red_magic", [] {
				return sdl::Texture::make("resources/textures/particles/magic/red.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return sdl::texture_manager()["particles/red_magic"];
	}
}
