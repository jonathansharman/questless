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

namespace questless
{
	void YellowMagic::subupdate()
	{
		_velocity.rotate(uniform(_turn_rate_min, _turn_rate_max));
		_angle = _velocity.angle();
	}

	sdl::Texture& YellowMagic::texture() const
	{
		static bool first_call = true;
		if (first_call) {
			sdl::texture_manager().add("particles/yellow_magic", [] {
				return sdl::Texture::make("resources/textures/particles/magic/yellow.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return sdl::texture_manager()["particles/yellow_magic"];
	}
}