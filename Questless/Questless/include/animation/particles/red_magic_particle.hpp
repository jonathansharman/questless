//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A particle for effects involving red magic.
	class red_magic_particle : public particle
	{
	public:
		red_magic_particle() : particle
			{ units::game_space::vector{0.0, 0.0}
			, (random_displacement(200.0) + units::game_space::vector{0.0, 150.0}) / 1.0s
			, units::game_space::vector{0.0, -300.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, scale{1.0}
			, units::game_space::scale_velocity{0.0}
			, lifetime{units::game_space::seconds{uniform(0.6, 1.0)}}
			}
		{}
	private:
		static constexpr units::game_space::radians _dtheta_max = 2.0 * units::game_space::radians::circle();

		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
