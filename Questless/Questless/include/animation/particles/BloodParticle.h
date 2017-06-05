//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"

namespace questless
{
	//! A blood drop particle.
	class BloodParticle : public Particle
	{
	public:
		BloodParticle() : Particle
			{ units::GameSpace::Vector{0.0, 0.0}
			, (random_displacement(160.0) + units::GameSpace::Vector{0.0, 150.0}) / 1.0s
			, units::GameSpace::Vector{0.0, -600.0} / 1.0s / 1.0s
			, random_angle()
			, units::GameSpace::Radians{0.0} / 1.0s
			, Scale{0.5}
			, units::GameScaleVelocity{Scale{0.0}}
			, Lifetime{units::GameSeconds{uniform(0.25, 0.75)}}
			}
		{
			_scale_velocity = units::GameScaleVelocity{Scale{-_scale / _lifetime.count()}};
		}
	private:
		bool fade_out() const final { return false; };

		bool face_towards_heading() const final { return true; }

		virtual bool ignore_color_mod() const { return false; }

		void particle_subupdate() final {}

		sdl::Texture const& texture() const final;
	};
}
