//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql {
	//! A particle for effects involving blue magic.
	class blue_magic_particle : public particle {
	public:
		blue_magic_particle();
	private:
		static constexpr world::radians _dtheta_max = 2.0 * world::radians::circle();

		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
