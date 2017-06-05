//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle for effects involving black magic.
	class BlackMagicParticle : public Particle
	{
	public:
		BlackMagicParticle() : Particle
			{ units::GameSpace::Vector{0.0, 0.0}
			, units::GameVelocity{units::GameSpace::Vector{random_angle(), uniform(5.0, 25.0)}}
			, units::GameAcceleration::zero()
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{2.0s}
			}
		{}
	private:
		static constexpr units::GameRadiansPerSec _turn_rate = units::GameSpace::Radians{4.0} / 1.0s;
		static constexpr units::GameSpace::Radians _dtheta_max = 2.0 * units::GameSpace::Radians::circle();
		static constexpr units::Hertz _acceleration_factor = 1.25_Hz;

		void particle_subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
