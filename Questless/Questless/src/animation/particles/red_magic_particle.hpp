//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! A particle for effects involving red magic.
	struct red_magic_particle : sprite_particle {
		red_magic_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec) -> void final {}
	};
}
