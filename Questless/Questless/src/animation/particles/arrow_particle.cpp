//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow_particle.hpp"

#include "utility/random.hpp"

using namespace ql::world::literals;

using namespace media;

namespace ql {
	namespace {
		sf::Texture texture() {
			static auto handle = the_texture_manager().add("resources/textures/particles/arrow.png");
			return the_texture_manager()[handle];
		}
	}

	arrow_particle::arrow_particle(world::point source, world::point target)
	    : sprite_particle{texture(), 0.0_s}
	    , _target{target} //
	{
		setPosition(to_sfml(source));
		angle = random_degrees();
		auto target_vector = target - source;
		if (target_vector == world::vector::zero()) {
			lifetime = 0.0_s;
		} else {
			// Set velocity towards the target.
			auto heading = target_vector / target_vector.length();
			velocity = heading * 1'000.0_world_length / 1.0_s;
			// Set lifetime / time left such that the arrow will disappear when it reaches the target.
			lifetime = target_vector.length() / velocity.length();
			time_left = lifetime;
		}
	}
}
