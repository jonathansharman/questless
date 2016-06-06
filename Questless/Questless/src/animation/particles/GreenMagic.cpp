/**
* @file    GreenMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the GreenMagic particle class.
*/

#include "animation/particles/GreenMagic.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	void GreenMagic::subupdate()
	{
		if (uniform(0.0, 1.0) < _inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.rotate(-_turn_rate);
		} else {
			_velocity.rotate(_turn_rate);
		}
	}

	sdl::Texture::ptr& GreenMagic::texture() const
	{
		static auto texture = sdl::Texture::make("resources/textures/particles/magic/green.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
