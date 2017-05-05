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
		//! @param position The particle's starting position.
		YellowMagicParticle(units::GamePoint position) : Particle
			{ position
			, units::GameVector{random_angle(), 100.0} / 1.0s
			, units::GameAcceleration::zero()
			, random_angle()
			, units::GameRadians{0.0} / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity::zero()
			, Lifetime{units::GameSeconds{uniform(0.8, 1.2)}}
			, MaxDisplacement{30.0}
			}
		{}
	private:
		static constexpr units::GameRadians _max_turn_rate = units::GameRadians::circle() / 6.0;

		void subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
