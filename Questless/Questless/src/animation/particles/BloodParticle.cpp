//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/BloodParticle.h"
#include "sdl/resources.h"

namespace questless
{
	sdl::Texture const& BloodParticle::texture() const
	{
		static sdl::TextureHandle handle = sdl::texture_manager().add("resources/textures/particles/blood.png");
		return sdl::texture_manager()[handle];
	}
}
