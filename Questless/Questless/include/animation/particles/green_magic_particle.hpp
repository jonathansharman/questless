//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql
{
	//! A particle for effects involving green magic.
	class green_magic_particle : public particle
	{
	public:
		green_magic_particle();
	private:
		bool _turning_right; //!< True if the green magic particle is turning to the right (CW), false if turning left (CCW).

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
