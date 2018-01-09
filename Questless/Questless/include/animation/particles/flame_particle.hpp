//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A particle of flame and smoke.
	class flame_particle : public particle
	{
	public:
		//! @param position The particle's starting position.
		flame_particle() : particle
			{ units::game_space::vector{0.0, 0.0}
			, units::game_space::velocity::zero()
			, units::game_space::vector{0.0, 30.0} / 1.0s / 1.0s
			, units::game_space::radians::zero()
			, units::game_space::radians_per_sec::zero()
			, scale{0.75}
			, units::game_space::scale_velocity::zero()
			, lifetime{units::game_space::seconds{uniform(1.0, 1.5)}}
			, max_displacement{5.0}
			}
		{
			_color_vector = units::colors::orange_vector();
		}
	private:
		static constexpr units::hertz _vx_pct_drag_rate = 1.8_Hz;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
