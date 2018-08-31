//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "yellow_magic_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	yellow_magic_particle::yellow_magic_particle() : particle
		{ game_space::vector::zero()
		, game_space::vector{random_angle(), 100.0} / 1.0s
		, game_space::acceleration::zero()
		, random_angle()
		, game_space::radians{0.0} / 1.0s
		, scale{1.0}
		, game_space::scale_velocity::zero()
		, lifetime{game_space::seconds{uniform(0.8, 1.2)}}
		, max_displacement{30.0}
		}
	{}

	void yellow_magic_particle::particle_subupdate() {
		constexpr game_space::radians max_turn_rate = game_space::radians::circle() / 6.0;
		_velocity.step().rotate(uniform(-1.0, 1.0) * max_turn_rate);
		_angle = _velocity.step().angle();
	}

	sdl::texture const& yellow_magic_particle::texture() const {
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/yellow.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
