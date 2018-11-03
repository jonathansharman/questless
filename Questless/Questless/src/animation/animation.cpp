//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation.hpp"

#include "game.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	void animation::update() {
		if (!_over && !_paused) {
			animation_subupdate();
		}
	}

	sec animation::elapsed_time() const {
		return target_frame_duration * time_scale.value();
	}
}
