//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "utility/utility.hpp"

namespace ql {
	//! A particle for effects involving white magic.
	struct white_magic_particle : sprite_particle {
		white_magic_particle();

	private:
		void particle_subupdate() final;

		media::texture const& texture() const final;
	};
}
