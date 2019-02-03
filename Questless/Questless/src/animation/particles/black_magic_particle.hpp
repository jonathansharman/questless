//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "utility/utility.hpp"

namespace ql {
	//! A particle for effects involving black magic.
	struct black_magic_particle : sprite_particle {
		black_magic_particle();

	private:
		static constexpr world::radians_per_sec _turn_rate = world::radians{4.0} / 1.0s;
		static constexpr world::radians _dtheta_max = 2.0 * world::radians::circle();
		static constexpr units::hertz _acceleration_factor = 1.25_Hz;

		void particle_subupdate() final;

		media::texture const& texture() const final;
	};
}
