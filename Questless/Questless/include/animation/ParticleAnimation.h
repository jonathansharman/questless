//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <deque>

#include "Animation.h"
#include "particles/Particle.h"
#include "utility/reference.h"

namespace questless
{
	//! Abstract base for animations composed of particle effects.
	class ParticleAnimation : public Animation
	{
	public:
		virtual ~ParticleAnimation() = default;

		void draw(units::ScreenPoint position) const final;

		void draw(units::GamePoint position, Camera const& camera, sdl::Color color = sdl::Color::white()) const final;
	protected:
		//! Adds @p particle to the front of the list of particles.
		void push_front(uptr<Particle> particle)
		{
			_particles.push_front(std::move(particle));
		}

		//! Adds @p particle to the back of the list of particles.
		void push_back(uptr<Particle> particle)
		{
			_particles.push_back(std::move(particle));
		}

		//! Particle animation subtype-specific update.
		virtual void particle_animation_subupdate() = 0;
	private:
		std::deque<uptr<Particle>> _particles;

		void animation_subupdate() final;
	};
}
