//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "red_magic_particle.hpp"

#include "utility/random.hpp"

using namespace units;

namespace ql {
	red_magic_particle::red_magic_particle() : particle
		{ world_space::vector::zero()
		, (random_displacement(200.0) + world_space::vector{0.0, 150.0}) / 1.0s
		, world_space::vector{0.0, -300.0} / 1.0s / 1.0s
		, random_angle()
		, uniform(-2.0, 2.0) * world::radians::circle() / 1.0s
		, 1.0
		, world_space::scale_velocity{0.0}
		, world_space::seconds{uniform(0.6, 1.0)}
		}
	{}
	media::texture const& red_magic_particle::texture() const {
		static auto texture_handle = media::the_texture_manager().add("resources/textures/particles/magic/red.png");
		return media::the_texture_manager()[texture_handle];
	}
}
