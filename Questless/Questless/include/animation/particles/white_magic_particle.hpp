//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A particle for effects involving white magic.
	class white_magic_particle : public particle
	{
	public:
		white_magic_particle();
	private:
		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
