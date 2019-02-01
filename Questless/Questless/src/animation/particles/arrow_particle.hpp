//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	//! An arrow in flight.
	class arrow_particle : public sprite_particle {
	public:
		//! @param source Where the arrow is shot from.
		//! @param target Where the arrow is headed towards.
		arrow_particle(world::point source, world::point target);
	private:
		world::point _target;

		bool fade_out() const final { return false; };

		bool face_towards_heading() const final { return true; }

		void sprite_particle_subupdate(sec) final {}
	};
}
