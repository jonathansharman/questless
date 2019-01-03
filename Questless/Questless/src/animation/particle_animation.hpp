//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <deque>

#include "animation.hpp"
#include "particles/particle.hpp"
#include "utility/reference.hpp"

namespace ql {
	//! Abstract base for animations composed of particle effects.
	class particle_animation : public animation {
	public:
		virtual ~particle_animation() = default;

		void draw(sdl::spaces::window::point position) const final;

		void draw(world::point position, camera const& camera, sdl::spaces::colors::color color_factor = sdl::spaces::colors::white()) const final;
	protected:
		//! Adds @p particle to the front of the list of particles.
		void push_front(uptr<particle> particle) {
			_particles.push_front(std::move(particle));
		}

		//! Adds @p particle to the back of the list of particles.
		void push_back(uptr<particle> particle) {
			_particles.push_back(std::move(particle));
		}

		//! Particle animation subtype-specific update.
		virtual void particle_animation_subupdate() = 0;
	private:
		std::deque<uptr<particle>> _particles;

		void animation_subupdate() final;
	};
}
