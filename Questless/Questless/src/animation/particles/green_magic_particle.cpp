//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "green_magic_particle.hpp"

#include "game.hpp"
#include "utility/random.hpp"

using namespace ql::world::literals;

using namespace media;
using namespace vecx;
using namespace vecx::literals;

namespace ql {
	namespace {
		sf::Texture const& texture() {
			static auto texture_handle = the_texture_manager().add("resources/textures/particles/magic/green.png");
			return the_texture_manager()[texture_handle];
		}
	}

	green_magic_particle::green_magic_particle()
	    : sprite_particle{texture(), uniform(1.8_s, 2.2_s)}
	    , _turning_right{random_bool()} //
	{
		displacement = world::vector::zero();
		velocity = random_displacement(20.0_world_length, 50.0_world_length) / 1.0_s;
		angle = random_degrees();
		angular_velocity = uniform(-2.0, 2.0) * circle_rad / 1.0_s;
		scale_velocity = -scale / lifetime;
	}

	void green_magic_particle::sprite_particle_subupdate(sec elapsed_time) {
		constexpr double inflection_probability = 0.1;
		if (bernoulli_trial(inflection_probability)) { _turning_right = !_turning_right; }

		constexpr auto turn_rate = circle_rad / 1.0_s;
		if (_turning_right) {
			velocity.rotate(-1.0_rad * turn_rate * elapsed_time);
		} else {
			velocity.rotate(turn_rate * elapsed_time);
		}
	}
}
