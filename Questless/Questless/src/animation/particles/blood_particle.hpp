//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	//! A blood drop particle.
	class blood_particle : public sprite_particle {
	public:
		blood_particle();

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
