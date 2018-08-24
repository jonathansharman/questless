//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql {
	//! A particle for effects involving red magic.
	class red_magic_particle : public particle {
	public:
		red_magic_particle();
	private:
		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
