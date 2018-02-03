//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql
{
	//! A blood drop particle.
	class blood_particle : public particle
	{
	public:
		blood_particle() : particle
			{ units::game_space::vector{0.0, 0.0} // displacement
			, units::game_space::vector::zero() / 1.0s // velocity
			, units::game_space::vector{0.0, -600.0} / 1.0s / 1.0s // acceleration
			, random_angle() // angle
			, units::game_space::radians{0.0} / 1.0s // angular velocity
			, scale{0.5f} // scale
			, units::game_space::scale_velocity{scale{0.0}} // scale velocity
			, lifetime{units::game_space::seconds{uniform(0.25, 0.75)}} // lifetime
			}
		{
			// Set the velocity to a factor of displacement plus some extra upwards velocity, to create an explosive effect.
			_velocity = (10.0 * _displacement + units::game_space::vector{0.0, 150.0}) / 1.0s;
			// Set the scale velocity so the blood particle reaches size zero at the same time it expires.
			_scale_velocity = units::game_space::scale_velocity{scale{-_scale / _lifetime.count()}};
		}
	private:
		bool fade_out() const final { return false; };

		bool face_towards_heading() const final { return true; }

		virtual bool ignore_color_mod() const { return false; }

		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
