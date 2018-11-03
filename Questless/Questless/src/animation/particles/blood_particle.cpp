//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "blood_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	blood_particle::blood_particle() : particle
		{ world_space::vector::zero()
		, world_space::vector::zero() / 1.0s
		, world_space::vector{0.0, -600.0} / 1.0s / 1.0s
		, random_angle()
		, world_space::radians{0.0} / 1.0s
		, 0.5
		, world_space::scale_velocity{0.0}
		, world_space::seconds{uniform(0.25, 0.75)}
		}
	{
		// Set the velocity to a factor of displacement plus some extra upwards velocity, to create an explosive effect.
		_velocity = (10.0 * _displacement + world_space::vector{0.0, 150.0}) / 1.0s;
		// Set the scale velocity so the blood particle reaches size zero at the same time it expires.
		_scale_velocity = world_space::scale_velocity{-_scale / _lifetime.count()};
	}

	sdl::texture const& blood_particle::texture() const {
		static sdl::texture_handle handle = sdl::the_texture_manager().add("resources/textures/particles/blood.png");
		return sdl::the_texture_manager()[handle];
	}
}
