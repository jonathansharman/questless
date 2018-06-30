//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/bleeding.hpp"

#include "animation/particles/blood_particle.hpp"

namespace ql
{
	void bleeding::particle_animation_subupdate()
	{
		_drops += _drops_per_update;
		while (_drops > 1.0) {
			push_front(umake<blood_particle>());
			--_drops;
		}
	}
}
