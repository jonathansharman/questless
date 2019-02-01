//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "white_magic_particle.hpp"

#include "game.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	white_magic_particle::white_magic_particle() : particle
		{ world_space::vector::zero()
		, random_displacement(80.0) / 1.0s
		, world_space::vector{0.0, 50.0} / 1.0s / 1.0s
		, random_angle()
		, uniform(-2.0, 2.0) * world_space::radians::circle() / 1.0s
		, 1.0
		, world_space::scale_velocity{0.0}
		, world_space::seconds{uniform(2.0, 2.5)}
		}
	{}

	void white_magic_particle::particle_subupdate() {
		constexpr units::hertz vx_pct_drag_rate = 1.8_Hz;
		_velocity.step().x() *= 1.0 - vx_pct_drag_rate * game::target_frame_duration;
	}

	media::texture const& white_magic_particle::texture() const {
		static auto texture_handle = media::the_texture_manager().add("resources/textures/particles/magic/white.png");
		return media::the_texture_manager()[texture_handle];
	}
}
