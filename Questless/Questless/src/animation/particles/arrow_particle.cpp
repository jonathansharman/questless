//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	arrow_particle::arrow_particle(world_space::point source, world_space::point target)
		: particle
			{ world_space::vector::zero()
			, world_space::vector::zero() / 1.0s
			, world_space::vector::zero() / 1.0s / 1.0s
			, random_angle<world_space>()
			, world_space::radians{0.0} / 1.0s
			, 1.0
			, world_space::scale_velocity{0.0}
			, world_space::seconds::zero()
			, max_displacement{0.0}
			}
		, _target{target}
	{
		auto target_vector = target - source;
		if (target_vector == world_space::vector::zero()) {
			_lifetime = 0.0_s;
		} else {
			// Set velocity towards the target.
			auto heading = target_vector / target_vector.length();
			_velocity = 1'000.0 * heading / 1.0s;
			// Set lifetime / time left such that the arrow will disappear when it reaches the target.
			_lifetime = sec{target_vector.length() / _velocity.step().length()};
			_time_left = _lifetime;
		}
	}

	sdl::texture const& arrow_particle::texture() const {
		static sdl::texture_handle handle = sdl::the_texture_manager().add("resources/textures/particles/arrow.png");
		return sdl::the_texture_manager()[handle];
	}
}
