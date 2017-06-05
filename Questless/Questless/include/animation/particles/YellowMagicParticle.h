//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

#include "animation/particles/Particle.h"

namespace questless
{
	//! A particle for effects involving yellow magic.
	class YellowMagicParticle : public Particle
	{
	public:
		YellowMagicParticle() : Particle
			{ units::GameSpace::Vector{0.0, 0.0}
			, units::GameSpace::Vector{random_angle(), 100.0} / 1.0s
			, units::GameAcceleration::zero()
			, random_angle()
			, units::GameSpace::Radians{0.0} / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity::zero()
			, Lifetime{units::GameSeconds{uniform(0.8, 1.2)}}
			, MaxDisplacement{30.0}
			}
		{}
	private:
		static constexpr units::GameSpace::Radians _max_turn_rate = units::GameSpace::Radians::circle() / 6.0;

		void particle_subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
