//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "white_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	white_magic_particle::white_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(2.0_s, 2.5_s), resources.white_magic} //
	{
		using namespace view::literals;

		velocity = random_displacement(80.0_px) / 1.0_s;
		acceleration = view::vector{0.0_px, 50.0_px} / 1.0_s / 1.0_s;
		angle = random_radians();
		angular_velocity = uniform(-2.0, 2.0) * vecx::circle_rad / 1.0_s;
	}

	auto white_magic_particle::sprite_particle_subupdate(sec elapsed_time) -> void {
		constexpr auto vx_pct_drag_rate = 1.8_hz;

		velocity[0] -= velocity[0] * vx_pct_drag_rate * elapsed_time;
	}
}
