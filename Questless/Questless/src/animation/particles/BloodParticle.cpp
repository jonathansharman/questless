/**
* @file    BloodParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Blood particle implementation.
*/

#include "animation/particles/BloodParticle.h"
#include "sdl/resources.h"

namespace questless
{
	sdl::Texture const& BloodParticle::texture() const
	{
		static sdl::Handle<sdl::Texture> handle = sdl::texture_manager().add("resources/textures/particles/blood.png");
		return sdl::texture_manager()[handle];
	}
}
