//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "blue_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	blue_magic_particle::blue_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(2.0_s, 2.4_s), resources.blue_magic} //
	{
		using namespace view::literals;

		constexpr auto dtheta_max = 2.0 * vecx::circle_rad;
		velocity = vecx::make_polar_vector(45.0_px, vecx::circle_rad / 6.0 * uniform(0, 6)) / 1.0_s;
		angle = random_radians();
		angular_velocity = uniform(-1.0, 1.0) * dtheta_max / 1.0_s;
	}
}
