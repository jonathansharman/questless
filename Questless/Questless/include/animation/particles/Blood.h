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
#include "utility/utility.h"

namespace questless
{
	class Blood : public Particle
	{
	public:
		/// @param position The particle's starting position.
		Blood(GamePoint position) : Particle
			{ position
			, (random_displacement(160.0) + GameVector{0.0, 150.0}) / 1.0s
			, GameVector{0.0, -600.0} / 1.0s / 1.0s
			, random_angle()
			, AngularVelocity{AngleRadians{0.0}}
			, Scale{0.5}
			, ScaleVelocity{Scale{0.0}}
			, Lifetime{seconds_f{uniform(0.25, 0.75)}}
			}
		{
			_scale_velocity = AngularVelocity{Scale{-_scale / _lifetime.count()}};
		}

		/// @param position The particle's starting position.
		static ptr make(GamePoint position) { return std::make_unique<Blood>(position); }
	private:
		bool fade_out() const override { return false; };

		bool face_towards_heading() const override { return true; }

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif
