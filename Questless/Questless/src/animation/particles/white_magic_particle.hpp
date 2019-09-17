//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "utility/utility.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! A particle for effects involving white magic.
	struct white_magic_particle : sprite_particle {
		white_magic_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec) -> void final;
	};
}
