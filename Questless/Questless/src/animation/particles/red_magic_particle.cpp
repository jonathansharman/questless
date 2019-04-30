//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "red_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	using namespace world::literals;

	red_magic_particle::red_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(0.6_s, 1.0_s), resources.red_magic} //
	{
		velocity = (random_displacement(200.0_world_length) + world::vector{0.0_world_length, 150.0_world_length}) / 1.0_s;
		acceleration = world::vector{0.0_world_length, -300.0_world_length} / 1.0_s / 1.0_s;
		angle = random_radians();
		angular_velocity = uniform(-2.0, 2.0) * vecx::circle_rad / 1.0_s;
	}
}
