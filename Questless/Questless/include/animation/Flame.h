//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ParticleAnimation.h"
#include "particles/FlameParticle.h"

namespace questless
{
	//! Creates sparks, smoke, and flame.
	class Flame : public ParticleAnimation
	{
	private:
		void particle_animation_subupdate() final
		{
			constexpr int n_flames = 2;
			for (int i = 0; i < n_flames; ++i) {
				push_front(std::make_unique<FlameParticle>());
			}
		}
	};
}
