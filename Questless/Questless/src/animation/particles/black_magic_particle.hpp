//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql {
	//! A particle for effects involving black magic.
	class black_magic_particle : public particle {
	public:
		black_magic_particle();
	private:
		static constexpr world::radians_per_sec _turn_rate = world::radians{4.0} / 1.0s;
		static constexpr world::radians _dtheta_max = 2.0 * world::radians::circle();
		static constexpr units::hertz _acceleration_factor = 1.25_Hz;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
