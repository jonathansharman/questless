//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! An arrow in flight.
	struct arrow_particle : sprite_particle {
		//! @param resources Particle resources used to animate this particle.
		//! @param source Where the arrow is shot from.
		//! @param target Where the arrow is headed towards.
		arrow_particle(rsrc::particle const& resources, view::point source, view::point target);

	private:
		view::point _target;

		auto fade_out() const -> bool final {
			return false;
		};

		auto face_towards_heading() const -> bool final {
			return true;
		}

		auto sprite_particle_subupdate(sec) -> void final {}
	};
}
