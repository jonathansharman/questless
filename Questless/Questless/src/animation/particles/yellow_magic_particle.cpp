//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "yellow_magic_particle.hpp"

#include "utility/random.hpp"

using namespace ql::world::literals;

using namespace media;
using namespace vecx;
using namespace vecx::literals;

namespace ql {
	namespace {
		sf::Texture const& texture() {
			static auto texture_handle = the_texture_manager().add("resources/textures/particles/magic/yellow.png");
			return the_texture_manager()[texture_handle];
		}
	}
	yellow_magic_particle::yellow_magic_particle() : sprite_particle{texture(), uniform(0.8_s, 1.2_s)} {
		displacement = random_displacement(30.0_world_length);
		velocity = make_polar_vector(100.0_world_length, random_radians()) / 1.0_s;
	}

	void yellow_magic_particle::sprite_particle_subupdate(sec elapsed_time) {
		constexpr radians max_turn_rate = circle_rad / 0.1_s;
		velocity.rotate(uniform(-max_turn_rate, max_turn_rate) * elapsed_time);
	}
}
