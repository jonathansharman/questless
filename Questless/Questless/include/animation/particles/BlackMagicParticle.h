/**
* @file    BlackMagicParticle.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the black magic particle class.
*/

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class BlackMagicParticle : public Particle
	{
	public:
		/// @param position The particle's starting position.
		BlackMagicParticle(units::GamePoint position) : Particle
			{ position
			, units::GameVelocity{units::GameVector{random_angle(), uniform(5.0, 25.0)}}
			, units::GameAcceleration::zero()
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{2.0s}
			}
		{}
	private:
		static constexpr units::GameRadiansPerSec _turn_rate = units::GameRadians{4.0} / 1.0s;
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();
		static constexpr units::Hertz _acceleration_factor = 1.25_Hz;

		void subupdate() override;

		sdl::Texture const& texture() const override;
	};
}
