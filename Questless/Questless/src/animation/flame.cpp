//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "flame.hpp"

#include "particles/flame_particle.hpp"

namespace ql {
	void flame::particle_animation_subupdate(sec elapsed_time) {
		_flames += flame_rate * elapsed_time;
		while (_flames > flames{1.0}) {
			particles.push_front(umake<flame_particle>(_resources));
			_flames -= flames{1.0};
		}
	}
}
