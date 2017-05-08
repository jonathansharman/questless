//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

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
		void animation_subupdate() final;

		//! Adds @p particle to the list of particles.
		void add(uptr<Particle> particle)
		{
			_particles.push_back(std::move(particle));
		}

		//! Particle animation subtype-specific update.
		virtual void particle_animation_subupdate() = 0;
	private:
		std::vector<uptr<Particle>> _particles;
	};
}
