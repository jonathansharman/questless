//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"

namespace ql
{
	//! Creates sparks, smoke, and flame.
	class flame : public particle_animation
	{
	private:
		void particle_animation_subupdate() final;
	};
}
