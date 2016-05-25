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
	void BlackMagic::update()
	{
		_velocity += _velocity * (ParticleK::black_acceleration_factor * frame_duration);
		_velocity.rotate(ParticleK::black_turn_rate * frame_duration);

		Particle::update();
	}

	Texture::ptr& BlackMagic::texture() const
	{
		static auto texture = Texture::make("resources/textures/particles/magic/black.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
