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
		Blood(PointF position) : Particle
			{ position
			, Velocity{random_displacement(_vi_max) + VectorF{0.0, _vyi_adjustment}}
			, _gravity
			, random_angle()
			, AngularVelocity{AngleRadians{0.0}}
			, Scale{0.5}
			, ScaleVelocity{Scale{0.0}}
			, Lifetime{seconds_f{uniform(_lifetime_min, _lifetime_max)}}
			}
		{
			_scale_velocity = AngularVelocity{Scale{-_scale / _lifetime.count()}};
		}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<Blood>(position); }
	private:
		static constexpr double _vyi_adjustment = -150.0;
		static constexpr double _vi_max = 160.0;
		static constexpr Acceleration _gravity{Velocity{VectorF{0.0, 600.0}}};
		static constexpr double _lifetime_min = 0.25;
		static constexpr double _lifetime_max = 0.75;

		bool fade_out() const override { return false; };

		bool face_towards_heading() const override { return true; }

		void subupdate() override {}

		sdl::Texture& texture() const override;
	};
}

#endif
