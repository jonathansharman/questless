//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/yellow_magic_particle.h"
#include "sdl/resources.h"

namespace ql
{
	void yellow_magic_particle::particle_subupdate()
	{
		_velocity.step().rotate(uniform(-1.0, 1.0) * _max_turn_rate);
		_angle = _velocity.step().angle();
	}

	sdl::texture const& yellow_magic_particle::texture() const
	{
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/yellow.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
