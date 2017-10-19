//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/black_magic_particle.hpp"
#include "game.hpp"
#include "sdl/resources.hpp"
using namespace units;

namespace ql
{
	void black_magic_particle::particle_subupdate()
	{
		_velocity *= 1.0 + _acceleration_factor * game::frame_duration;
		_velocity.step().rotate(game_space::radians{_turn_rate * game::frame_duration});
	}

	sdl::texture const& black_magic_particle::texture() const
	{
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/black.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
