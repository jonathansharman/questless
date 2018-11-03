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
		static constexpr units::world_space::radians_per_sec _turn_rate = units::world_space::radians{4.0} / 1.0s;
		static constexpr units::world_space::radians _dtheta_max = 2.0 * units::world_space::radians::circle();
		static constexpr units::hertz _acceleration_factor = 1.25_Hz;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
