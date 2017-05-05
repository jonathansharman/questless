//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle for effects involving red magic.
	class RedMagicParticle : public Particle
	{
	public:
		//! @param position The particle's starting position.
		RedMagicParticle(units::GamePoint position) : Particle
			{ position
			, (random_displacement(200.0) + units::GameVector{0.0, 150.0}) / 1.0s
			, units::GameVector{0.0, -300.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(0.6, 1.0)}}
			}
		{}
	private:
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() final {}

		sdl::Texture const& texture() const final;
	};
}
