//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

using namespace ql::world::literals;

using namespace media;

namespace ql {
	arrow_particle::arrow_particle(rsrc::particle const& resources, world::point source, world::point target)
		: sprite_particle{0.0_s, resources.arrow}
		, _target{target} //
	{
		setPosition(to_sfml(source));
		angle = random_radians();
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
