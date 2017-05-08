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
	public:
		Flame(RegionTileCoords coords) : _coords{coords} {}
	protected:
		void particle_animation_subupdate() final
		{
			constexpr int n_flames = 2;
			for (int i = 0; i < n_flames; ++i) {
				add(std::make_unique<FlameParticle>(units::Layout::dflt().to_world(_coords)));
			}
		}
	private:
		RegionTileCoords _coords;
	};
}
