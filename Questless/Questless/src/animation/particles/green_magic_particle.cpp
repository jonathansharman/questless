//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/green_magic_particle.h"
#include "game.h"
#include "sdl/resources.h"

namespace ql
{
	void green_magic_particle::particle_subupdate()
	{
		if (uniform(0.0, 1.0) < _inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.step().rotate(units::game_space::radians{-1.0 * _turn_rate * game::frame_duration});
		} else {
			_velocity.step().rotate(units::game_space::radians{_turn_rate * game::frame_duration});
		}
	}

	sdl::texture const& green_magic_particle::texture() const
	{
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/green.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
