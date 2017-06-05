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
			{ units::GameSpace::Vector{0.0, 0.0}
			, units::GameVelocity::zero()
			, units::GameSpace::Vector{0.0, 30.0} / 1.0s / 1.0s
			, units::GameSpace::Radians::zero()
			, units::GameRadiansPerSec::zero()
			, Scale{0.75}
			, units::GameScaleVelocity::zero()
			, Lifetime{units::GameSeconds{uniform(1.0, 1.5)}}
			, MaxDisplacement{5.0}
			}
		{
			_color_factor = units::colors::ColorFactor{1.0f, 0.5f, 0.0f};
		}
	private:
		static constexpr units::Hertz _vx_percent_drag_rate = 1.8_Hz;

		void particle_subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
