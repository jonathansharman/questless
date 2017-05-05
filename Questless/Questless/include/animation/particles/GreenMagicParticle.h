//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle for effects involving green magic.
	class GreenMagicParticle : public Particle
	{
	public:
		//! @param position The particle's starting position.
		GreenMagicParticle(units::GamePoint position)
			: Particle
				{ position
				, random_displacement(20.0, 50.0) / 1.0s
				, units::GameAcceleration::zero()
				, random_angle()
				, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
				, Scale{1.0}
				, units::GameScaleVelocity{0.0}
				, Lifetime{units::GameSeconds{uniform(1.8, 2.2)}}
				}
			, _turning_right{random_bool()}
		{
			_scale_velocity = units::GameScaleVelocity{Scale{-_scale / _lifetime.count()}};
		}
	private:
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();
		static constexpr double _inflection_probability = 0.1;
		static constexpr units::GameRadiansPerSec _turn_rate = units::GameRadians::circle() / 1.0s;

		bool _turning_right; //!< True if the green magic particle is turning to the right (CW), false if turning left (CCW).

		void subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
