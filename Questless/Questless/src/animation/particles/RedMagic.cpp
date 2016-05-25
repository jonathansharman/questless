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
	void RedMagic::update()
	{
		_velocity.y += Hertz{ParticleK::red_gravity * frame_duration};

		Particle::update();
	}

	Texture::ptr& RedMagic::texture() const
	{
		static auto texture = Texture::make("resources/textures/particles/magic/red.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}
