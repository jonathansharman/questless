//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	//! A particle for effects involving blue magic.
	struct blue_magic_particle : sprite_particle {
		blue_magic_particle();

	private:
		static constexpr world::radians _dtheta_max = 2.0 * world::radians::circle();

		void particle_subupdate() final {}

		media::texture const& texture() const final;
	};
}
