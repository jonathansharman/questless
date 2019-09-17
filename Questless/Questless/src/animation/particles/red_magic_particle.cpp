//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "red_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	using namespace view::literals;

	red_magic_particle::red_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(0.6_s, 1.0_s), resources.red_magic} //
	{
		velocity = (random_displacement(200.0_px) + view::vector{0.0_px, 150.0_px}) / 1.0_s;
		acceleration = view::vector{0.0_px, -300.0_px} / 1.0_s / 1.0_s;
		angle = random_radians();
		angular_velocity = uniform(-2.0, 2.0) * vecx::circle_rad / 1.0_s;
	}
}
