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
	void GreenMagic::update()
	{
		if (uniform(0.0, 1.0) < ParticleK::green_inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.rotate(-ParticleK::green_turn_rate);
		} else {
			_velocity.rotate(ParticleK::green_turn_rate);
		}

		Particle::update();
	}

	Texture::ptr& GreenMagic::texture() const
	{
		static auto texture = Texture::make("resources/textures/particles/magic/green.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
