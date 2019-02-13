//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "black_magic_particle.hpp"

#include "game.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	black_magic_particle::black_magic_particle()
		: particle{world_space::vector::zero(),
			  world_space::velocity{world_space::vector{random_angle(), uniform(5.0, 25.0)}},
			  world_space::acceleration::zero(),
			  random_angle(),
			  uniform(-1.0, 1.0) * _dtheta_max / 1.0s,
			  1.0,
			  world_space::scale_velocity{0.0},
			  2.0s} {}

	void black_magic_particle::particle_subupdate(sec elapsed_time) {
		velocity *= 1.0 + _acceleration_factor * elapsed_time;
		velocity.rotate(_turn_rate * elapsed_time);
	}

	media::texture const& black_magic_particle::texture() const {
		static auto texture_handle = media::the_texture_manager().add("resources/textures/particles/magic/black.png");
		return media::the_texture_manager()[texture_handle];
	}
}
