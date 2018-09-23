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
			{ game_space::vector::zero()
			, random_displacement(20.0, 50.0) / 1.0s
			, game_space::acceleration::zero()
			, random_angle()
			, uniform(-2.0, 2.0) * units::game_space::radians::circle() / 1.0s
			, 1.0
			, game_space::scale_velocity{0.0}
			, game_space::seconds{uniform(1.8, 2.2)}
			}
		, _turning_right{random_bool()}
	{
		_scale_velocity = game_space::scale_velocity{-_scale / _lifetime.count()};
	}

	void green_magic_particle::particle_subupdate() {
		constexpr double inflection_probability = 0.1;
		if (bernoulli_trial(inflection_probability)) {
			_turning_right = !_turning_right;
		}

		constexpr auto turn_rate = game_space::radians::circle() / 1.0s;
		if (_turning_right) {
			_velocity.step().rotate(game_space::radians{-1.0 * turn_rate * game::frame_duration});
		} else {
			_velocity.step().rotate(game_space::radians{turn_rate * game::frame_duration});
		}
	}

	sdl::texture const& green_magic_particle::texture() const {
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/green.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
