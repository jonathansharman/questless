//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "utility/utility.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! A particle for effects involving black magic.
	struct black_magic_particle : sprite_particle {
		black_magic_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec elapsed_time) -> void final;
	};
}
