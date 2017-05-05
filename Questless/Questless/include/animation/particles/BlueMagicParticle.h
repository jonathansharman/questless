//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle for effects involving blue magic.
	class BlueMagicParticle : public Particle
	{
	public:
		//! @param position The particle's starting position.
		BlueMagicParticle(units::GamePoint position) : Particle
			{ position
			, units::GameVector{units::GameRadians::circle() / 6.0 * uniform(0, 6), 45.0} / 1.0s
			, units::GameAcceleration::zero()
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(2.0, 2.4)}}
			}
		{}
	private:
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() final {}

		sdl::Texture const& texture() const final;
	};
}
