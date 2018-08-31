//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "red_magic_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	red_magic_particle::red_magic_particle() : particle
		{ game_space::vector::zero()
		, (random_displacement(200.0) + game_space::vector{0.0, 150.0}) / 1.0s
		, game_space::vector{0.0, -300.0} / 1.0s / 1.0s
		, random_angle()
		, uniform(-2.0, 2.0) * units::game_space::radians::circle() / 1.0s
		, scale{1.0}
		, game_space::scale_velocity{0.0}
		, lifetime{game_space::seconds{uniform(0.6, 1.0)}}
		}
	{}
	sdl::texture const& red_magic_particle::texture() const {
		static auto texture_handle = sdl::the_texture_manager().add("resources/textures/particles/magic/red.png");
		return sdl::the_texture_manager()[texture_handle];
	}
}
