//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "green_magic_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

using namespace vecx;
using namespace vecx::literals;

namespace ql {
	using namespace view::literals;

	green_magic_particle::green_magic_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(1.8_s, 2.2_s), resources.green_magic}
		, _turning_right{coin_flip()} //
	{
		displacement = view::vector::zero();
		velocity = random_displacement(20.0_px, 50.0_px) / 1.0_s;
		angle = random_radians();
		angular_velocity = uniform(-2.0, 2.0) * circle_rad / 1.0_s;
		scale_velocity = -scale / lifetime;
	}

	auto green_magic_particle::sprite_particle_subupdate(sec elapsed_time) -> void {
		constexpr double inflection_probability = 0.1;
		if (bernoulli_trial(inflection_probability)) { _turning_right = !_turning_right; }

		constexpr auto turn_rate = circle_rad / 1.0_s;
		if (_turning_right) {
			velocity.rotate(-turn_rate * elapsed_time);
		} else {
			velocity.rotate(turn_rate * elapsed_time);
		}
	}
}
