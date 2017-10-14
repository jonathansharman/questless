//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.h"

namespace ql
{
	//! A blood drop particle.
	class blood_particle : public particle
	{
	public:
		blood_particle() : particle
			{ units::game_space::vector{0.0, 0.0}
			, (random_displacement(160.0) + units::game_space::vector{0.0, 150.0}) / 1.0s
			, units::game_space::vector{0.0, -600.0} / 1.0s / 1.0s
			, random_angle()
			, units::game_space::radians{0.0} / 1.0s
			, scale{0.5f}
			, units::game_space::scale_velocity{scale{0.0}}
			, lifetime{units::game_space::seconds{uniform(0.25, 0.75)}}
			}
		{
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
