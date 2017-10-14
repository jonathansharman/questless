//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.h"
#include "utility/utility.h"

namespace ql
{
	//! A particle for effects involving white magic.
	class white_magic_particle : public particle
	{
	public:
		white_magic_particle() : particle
			{ units::game_space::vector{0.0, 0.0}
			, random_displacement(80.0) / 1.0s
			, units::game_space::vector{0.0, 50.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, scale{1.0}
			, units::game_space::scale_velocity{0.0}
			, lifetime{units::game_space::seconds{uniform(2.0, 2.5)}}
			}
		{}
	private:
		static constexpr units::hertz _vx_percent_drag_rate = 1.8_Hz;
		static constexpr units::game_space::radians _dtheta_max = 2.0 * units::game_space::radians::circle();

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
