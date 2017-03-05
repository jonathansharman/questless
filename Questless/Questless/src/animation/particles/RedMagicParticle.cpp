/**
* @file    RedMagicParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Red magic particle implementation.
*/

#include "animation/particles/RedMagicParticle.h"
#include "sdl/resources.h"

namespace questless
{
	sdl::Texture const& RedMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/red.png");
		return sdl::texture_manager()[texture_handle];
	}
}
