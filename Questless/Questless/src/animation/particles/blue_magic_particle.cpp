//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/blue_magic_particle.hpp"
#include "sdl/resources.hpp"

namespace ql
{
	sdl::texture const& blue_magic_particle::texture() const
	{
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/blue.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
