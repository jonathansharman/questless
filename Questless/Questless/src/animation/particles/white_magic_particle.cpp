//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/white_magic_particle.hpp"
#include "game.hpp"
#include "sdl/resources.hpp"

namespace ql
{
	void white_magic_particle::particle_subupdate()
	{
		_velocity.step().x() *= 1.0 - _vx_percent_drag_rate * game::frame_duration;
	}

	sdl::texture const& white_magic_particle::texture() const
	{
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/white.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
