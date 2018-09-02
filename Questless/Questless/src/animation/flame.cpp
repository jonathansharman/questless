//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "flame.hpp"

#include "animation/particles/flame_particle.hpp"

namespace ql {
	void flame::particle_animation_subupdate() {
		constexpr int n_flames = 2;
		for (int i = 0; i < n_flames; ++i) {
			push_front(umake<flame_particle>());
		}
	}
}
