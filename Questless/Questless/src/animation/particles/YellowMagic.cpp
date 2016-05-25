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
	void YellowMagic::update()
	{
		_velocity.rotate(uniform(ParticleK::yellow_turn_rate_min, ParticleK::yellow_turn_rate_max));
		_angle = _velocity.angle();

		Particle::update();
	}

	Texture::ptr& YellowMagic::texture() const
	{
		static auto texture = Texture::make("resources/textures/particles/magic/yellow.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}