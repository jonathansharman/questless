//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle of flame and smoke.
	class FlameParticle : public Particle
	{
	public:
		//! @param position The particle's starting position.
		FlameParticle() : Particle
			{ units::GameVector{0.0, 0.0}
			, units::GameVelocity::zero()
			, units::GameVector{0.0, 30.0} / 1.0s / 1.0s
			, units::GameRadians::zero()
			, units::GameRadiansPerSec::zero()
			, Scale{0.75}
			, units::GameScaleVelocity::zero()
			, Lifetime{units::GameSeconds{uniform(1.0, 1.5)}}
			, MaxDisplacement{5.0}
			}
		{
			_color = sdl::Color{255, 128, 0};
		}
	private:
		static constexpr units::Hertz _vx_percent_drag_rate = 1.8_Hz;

		void particle_subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
