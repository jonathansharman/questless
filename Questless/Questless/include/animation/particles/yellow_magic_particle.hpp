//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

#include "utility/random.hpp"

namespace ql
{
	//! A particle for effects involving yellow magic.
	class yellow_magic_particle : public particle
	{
	public:
		yellow_magic_particle();
	private:
		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
