//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "blood_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	blood_particle::blood_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(0.25_s, 0.75_s), resources.blood} //
	{
		using namespace world::literals;

		scale = {0.5};
		// Set the velocity to a factor of displacement plus some extra upwards velocity, to create an explosive effect.
		velocity = (10.0f * displacement + world::vector{0.0_world_length, 150.0_world_length}) / 1.0_s;
		acceleration = world::vector{0.0_world_length, -600.0_world_length} / 1.0_s / 1.0_s;
		// Set the scale velocity so the blood particle reaches size zero at the same time it expires.
		scale_velocity = -scale / lifetime;
	}
}
