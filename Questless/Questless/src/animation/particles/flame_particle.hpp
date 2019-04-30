//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! A particle of flame and smoke.
	struct flame_particle : sprite_particle {
		flame_particle(rsrc::particle const& resources);

	private:
		void sprite_particle_subupdate(sec elapsed_time) final;
	};
}
