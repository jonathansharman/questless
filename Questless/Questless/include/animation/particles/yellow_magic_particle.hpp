//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

#include "animation/particles/particle.hpp"

namespace ql
{
	//! A particle for effects involving yellow magic.
	class yellow_magic_particle : public particle
	{
	public:
		yellow_magic_particle() : particle
			{ units::game_space::vector{0.0, 0.0}
			, units::game_space::vector{random_angle(), 100.0} / 1.0s
			, units::game_space::acceleration::zero()
			, random_angle()
			, units::game_space::radians{0.0} / 1.0s
			, scale{1.0}
			, units::game_space::scale_velocity::zero()
			, lifetime{units::game_space::seconds{uniform(0.8, 1.2)}}
			, max_displacement{30.0}
			}
		{}
	private:
		static constexpr units::game_space::radians _max_turn_rate = units::game_space::radians::circle() / 6.0;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
