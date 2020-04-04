//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! A blood drop particle.
	struct blood_particle : sprite_particle {
		blood_particle(rsrc::particle const& resources);

	private:
		auto fade_out() const -> bool final {
			return false;
		};

		auto face_towards_heading() const -> bool final {
			return true;
		}

		auto sprite_particle_subupdate(sec) -> void final {}
	};
}
