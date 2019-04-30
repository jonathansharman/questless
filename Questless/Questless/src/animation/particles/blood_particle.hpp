//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	namespace rsrc {
		struct particle;
	}

	//! A blood drop particle.
	struct blood_particle : sprite_particle {
		blood_particle(rsrc::particle const& resources);

	private:
		bool fade_out() const final {
			return false;
		};

		bool face_towards_heading() const final {
			return true;
		}

		void sprite_particle_subupdate(sec) final {}
	};
}
