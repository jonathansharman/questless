//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A particle for effects involving green magic.
	class green_magic_particle : public particle
	{
	public:
		green_magic_particle()
			: particle
				{ units::game_space::vector{0.0, 0.0}
				, random_displacement(20.0, 50.0) / 1.0s
				, units::game_space::acceleration::zero()
				, random_angle()
				, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
				, scale{1.0}
				, units::game_space::scale_velocity{0.0}
				, lifetime{units::game_space::seconds{uniform(1.8, 2.2)}}
				}
			, _turning_right{random_bool()}
		{
			_scale_velocity = units::game_space::scale_velocity{scale{-_scale / _lifetime.count()}};
		}
	private:
		static constexpr units::game_space::radians _dtheta_max = 2.0 * units::game_space::radians::circle();
		static constexpr double _inflection_probability = 0.1;
		static constexpr units::game_space::radians_per_sec _turn_rate = units::game_space::radians::circle() / 1.0s;

		bool _turning_right; //!< True if the green magic particle is turning to the right (CW), false if turning left (CCW).

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
