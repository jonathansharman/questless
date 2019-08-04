//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "black_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

using namespace vecx::literals;

namespace ql {
	using namespace view::literals;

	black_magic_particle::black_magic_particle(rsrc::particle const& resources)
		: sprite_particle{2.0_s, resources.black_magic} //
	{
		constexpr auto dtheta_max = 2.0 * vecx::circle_rad;
		constexpr auto min_vel = 5.0_px;
		constexpr auto max_vel = 25.0_px;

		velocity = vecx::make_polar_vector(uniform(min_vel, max_vel), random_radians()) / 1.0_s;
		angle = random_radians();
		uniform(-dtheta_max, dtheta_max) * dtheta_max / 1.0_s;
	}

	void black_magic_particle::sprite_particle_subupdate(sec elapsed_time) {
		constexpr auto acceleration_factor = 1.25_hz;
		constexpr auto turn_rate = 4.0_rad / 1.0_s;

		velocity += velocity * acceleration_factor * elapsed_time;
		velocity.rotate(turn_rate * elapsed_time);
	}
}
