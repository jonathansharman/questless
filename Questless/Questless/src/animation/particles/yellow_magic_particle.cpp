//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "yellow_magic_particle.hpp"

#include "rsrc/particle.hpp"

#include "utility/random.hpp"

namespace ql {
	using namespace view::literals;

	yellow_magic_particle::yellow_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(0.8_s, 1.2_s), resources.yellow_magic} //
	{
		displacement = random_displacement(30.0_px);
		velocity = make_polar_vector(100.0_px, random_radians()) / 1.0_s;
	}

	void yellow_magic_particle::sprite_particle_subupdate(sec elapsed_time) {
		constexpr auto max_turn_rate = vecx::circle_rad / 0.1_s;
		velocity.rotate(uniform(-max_turn_rate, max_turn_rate) * elapsed_time);
	}
}
