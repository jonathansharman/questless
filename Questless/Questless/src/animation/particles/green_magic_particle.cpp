//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "green_magic_particle.hpp"

#include "game.hpp"
#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	green_magic_particle::green_magic_particle()
		: particle
			{ world_space::vector::zero()
			, random_displacement<world_space>(20.0, 50.0) / 1.0s
			, world_space::acceleration::zero()
			, random_angle<world_space>()
			, uniform(-2.0, 2.0) * units::world_space::radians::circle() / 1.0s
			, 1.0
			, world_space::scale_velocity{0.0}
			, sec{uniform(1.8, 2.2)}
			}
		, _turning_right{random_bool()}
	{
		_scale_velocity = world_space::scale_velocity{-_scale / _lifetime};
	}

	void green_magic_particle::particle_subupdate() {
		constexpr double inflection_probability = 0.1;
		if (bernoulli_trial(inflection_probability)) {
			_turning_right = !_turning_right;
		}

		constexpr auto turn_rate = world_space::radians::circle() / 1.0s;
		if (_turning_right) {
			_velocity.step().rotate(world_space::radians{-1.0 * turn_rate * game::target_frame_duration});
		} else {
			_velocity.step().rotate(world_space::radians{turn_rate * game::frame_duration});
		}
	}

	sdl::texture const& green_magic_particle::texture() const {
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/green.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
