//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <deque>

#include "animation.h"
#include "particles/particle.h"
#include "utility/reference.h"

namespace ql
{
	//! Abstract base for animations composed of particle effects.
	class particle_animation : public animation
	{
	public:
		virtual ~particle_animation() = default;

		void draw(units::screen_space::point position) const final;

		void draw(units::game_space::point position, camera const& camera, units::colors::color_factor color_factor = units::colors::white_factor()) const final;
	protected:
		//! Adds @p particle to the front of the list of particles.
		void push_front(uptr<particle> particle)
		{
			_particles.push_front(std::move(particle));
		}

		//! Adds @p particle to the back of the list of particles.
		void push_back(uptr<particle> particle)
		{
			_particles.push_back(std::move(particle));
		}

		//! Particle animation subtype-specific update.
		virtual void particle_animation_subupdate() = 0;
	private:
		std::deque<uptr<particle>> _particles;

		void animation_subupdate() final;
	};
}
