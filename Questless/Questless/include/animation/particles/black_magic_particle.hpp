//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A particle for effects involving black magic.
	class black_magic_particle : public particle
	{
	public:
		black_magic_particle() : particle
			{ units::game_space::vector::zero()
			, units::game_space::velocity{units::game_space::vector{random_angle(), uniform(5.0, 25.0)}}
			, units::game_space::acceleration::zero()
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, scale{1.0}
			, units::game_space::scale_velocity{0.0}
			, lifetime{2.0s}
			}
		{}
	private:
		static constexpr units::game_space::radians_per_sec _turn_rate = units::game_space::radians{4.0} / 1.0s;
		static constexpr units::game_space::radians _dtheta_max = 2.0 * units::game_space::radians::circle();
		static constexpr units::hertz _acceleration_factor = 1.25_Hz;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
