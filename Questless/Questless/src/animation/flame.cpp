//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "flame.hpp"

#include "particles/flame_particle.hpp"

namespace ql {
	flame::flame(rsrc::particle const& resources) : _rsrc{&resources} {}

	auto flame::particle_animation_subupdate(sec elapsed_time) -> void {
		_flames += flame_rate * elapsed_time;
		while (_flames > flames{1.0}) {
			particles.push_front(umake<flame_particle>(*_rsrc));
			_flames -= flames{1.0};
		}
		//! @todo This can be done without a loop using fmod.
	}
}
