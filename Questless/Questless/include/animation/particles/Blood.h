/**
* @file    Blood.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A blood drop particle.
*/

#ifndef BLOOD_H
#define BLOOD_H

#include "animation/particles/Particle.h"

namespace questless
{
	class Blood : public Particle
	{
	public:
		/// @param position The particle's starting position.
		Blood(units::GamePoint position) : Particle
			{ position
			, (random_displacement(160.0) + units::GameVector{0.0, 150.0}) / 1.0s
			, units::GameVector{0.0, -600.0} / 1.0s / 1.0s
			, random_angle()
			, units::GameRadians{0.0} / 1.0s
			, Scale{0.5}
			, units::GameScaleVelocity{Scale{0.0}}
			, Lifetime{units::GameSeconds{uniform(0.25, 0.75)}}
			}
		{
			_scale_velocity = units::GameScaleVelocity{Scale{-_scale / _lifetime.count()}};
		}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position) { return std::make_unique<Blood>(position); }
	private:
		bool fade_out() const override { return false; };

		bool face_towards_heading() const override { return true; }

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif
