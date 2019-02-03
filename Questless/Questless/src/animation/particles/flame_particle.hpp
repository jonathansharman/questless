//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	//! A particle of flame and smoke.
	struct flame_particle : sprite_particle {
		flame_particle();

	private:
		void sprite_particle_subupdate(sec elapsed_time) final;
	};
}
