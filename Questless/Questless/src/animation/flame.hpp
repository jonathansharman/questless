//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! Creates sparks, smoke, and flame.
	struct flame : particle_animation {
		flame(rsrc::particle const& resources) : _rsrc{resources} {}

		//! Unit of flames, for use in flame animations.
		using flames = cancel::quantity<double, cancel::unit_t<struct flame_particle_tag>>;

		//! The number of flames to create per second. Can be less than one.
		cancel::quotient_t<flames, sec> flame_rate = flames{50} / 1.0_s;

	private:
		rsrc::particle const& _rsrc;

		//! The current accumulation of flames.
		flames _flames{0.0};

		auto particle_animation_subupdate(sec elapsed_time) -> void final;
	};
}
