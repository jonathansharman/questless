//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "blue_magic_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	blue_magic_particle::blue_magic_particle() : particle
		{ world_space::vector::zero()
		, world_space::vector{world_space::radians::circle() / 6.0 * uniform(0, 6), 45.0} / 1.0s
		, world_space::acceleration::zero()
		, random_angle()
		, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
		, 1.0
		, world_space::scale_velocity{0.0}
		, world_space::seconds{uniform(2.0, 2.4)}
		}
	{}
	sdl::texture const& blue_magic_particle::texture() const {
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/blue.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
