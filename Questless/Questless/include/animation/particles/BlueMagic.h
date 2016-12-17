/**
* @file    BlueMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BlueMagic particle class.
*/

#ifndef BLUE_MAGIC_H
#define BLUE_MAGIC_H

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class BlueMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		BlueMagic(PointF position) : Particle
			{ position
			, Velocity{VectorF{AngleRadians{tau / 6.0 * uniform(0, 6)}, Length{_v}}}
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(_lifetime_min, _lifetime_max)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<BlueMagic>(position); }
	private:
		static constexpr double _v = 45.0;
		static constexpr double _dtheta_max = 2.0 * tau;
		static constexpr double _lifetime_min = 2.0;
		static constexpr double _lifetime_max = 2.4;

		void subupdate() override {}

		sdl::Texture& texture() const override;
	};
}

#endif