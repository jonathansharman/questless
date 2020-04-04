//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"
#include "utility/random.hpp"

namespace ql {
	//! A particle for effects involving yellow magic.
	struct yellow_magic_particle : sprite_particle {
		yellow_magic_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec elapsed_time) -> void final;
	};
}
