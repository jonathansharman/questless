//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"

namespace ql {
	//! Creates sparks, smoke, and flame.
	struct flame : particle_animation {
		//! Unit of flames, for use in flame animations.
		using flames = cancel::quantity<double, cancel::unit_t<struct flame_particle_tag>>;

		//! The number of flames to create per second. Can be less than one.
		decltype(flames{} / sec{}) flame_rate = flames{2} / 1.0_s;

	private:
		//! The current accumulation of flames.
		flames _flames{0.0};

		void particle_animation_subupdate(sec elapsed_time) final;
	};
}
