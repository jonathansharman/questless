/**
* @file    WhiteMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WhiteMagic particle class.
*/

#ifndef WHITE_MAGIC_H
#define WHITE_MAGIC_H

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class WhiteMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		WhiteMagic(PointF position) : Particle
			{ position
			, random_displacement(_vi_max) / 1.0s
			, _gravity
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(_lifetime_min, _lifetime_max)}}
			}
		{
			Rate<VectorF, double> vel{random_displacement(_vi_max) / 1.0s};
		}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<WhiteMagic>(position); }
	private:
		static constexpr Acceleration _gravity{Velocity{VectorF{0.0, -50.0}}};
		static constexpr double _vi_max = 80.0;
		static constexpr Hertz _vx_percent_drag_rate = 1.8_Hz;
		static constexpr double _dtheta_max = 2.0 * tau;
		static constexpr double _lifetime_min = 2.0;
		static constexpr double _lifetime_max = 2.5;

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif