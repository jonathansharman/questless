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
	void WhiteMagic::update()
	{
		_velocity.y += Hertz{ParticleK::white_gravity * frame_duration};
		_velocity.x *= ParticleK::white_vx_factor_per_frame;

		Particle::update();
	}

	Texture::ptr& WhiteMagic::texture() const
	{
		static auto texture = Texture::make("resources/textures/particles/magic/white.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		return texture;
	}
}