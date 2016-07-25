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

using namespace sdl;

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

	Texture& GreenMagic::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/magic/green.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}
