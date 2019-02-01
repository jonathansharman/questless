//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "utility/random.hpp"

namespace ql {
	//! A particle for effects involving yellow magic.
	class yellow_magic_particle : public sprite_particle {
	public:
		yellow_magic_particle();

	private:
		void sprite_particle_subupdate(sec elapsed_time) final;
	};
}
