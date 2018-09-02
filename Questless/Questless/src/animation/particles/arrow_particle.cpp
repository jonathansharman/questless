//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	arrow_particle::arrow_particle(game_space::point source, game_space::point target)
		: particle
			{ game_space::vector::zero() // displacement
			, game_space::vector::zero() / 1.0s // velocity
			, game_space::vector::zero() / 1.0s / 1.0s // acceleration
			, random_angle() // angle
			, game_space::radians{0.0} / 1.0s // angular velocity
			, scale{1.0f} // scale
			, game_space::scale_velocity{scale{0.0}} // scale velocity
			, lifetime{game_space::seconds::zero()} // lifetime
			, max_displacement{0.0}
			}
		, _target{target}
	{
		auto target_vector = target - source;
		if (target_vector == game_space::vector::zero()) {
			_lifetime = game_space::seconds::zero();
		} else {
			// Set velocity towards the target.
			auto heading = target_vector / target_vector.length();
			_velocity = 1'000.0 * heading / 1.0s;
			// Set lifetime / time left such that the arrow will disappear when it reaches the target.
			_lifetime = game_space::seconds{target_vector.length() / _velocity.step().length()};
			_time_left = _lifetime;
		}
	}

	sdl::texture const& arrow_particle::texture() const {
		static sdl::texture_handle handle = sdl::the_texture_manager().add("resources/textures/particles/arrow.png");
		return sdl::the_texture_manager()[handle];
	}
}
