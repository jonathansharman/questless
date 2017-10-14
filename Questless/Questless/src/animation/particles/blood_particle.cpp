//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/blood_particle.h"
#include "sdl/resources.h"

namespace ql
{
	sdl::texture const& blood_particle::texture() const
	{
		static sdl::texture_handle handle = sdl::the_texture_manager().add("resources/textures/particles/blood.png");
		return sdl::the_texture_manager()[handle];
	}
}
