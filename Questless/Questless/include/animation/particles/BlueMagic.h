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
			, VectorF{AngleRadians{tau / 6.0 * uniform(0, 6)}, Length{45.0}} / 1.0s
			, Acceleration::zero()
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(2.0, 2.4)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<BlueMagic>(position); }
	private:
		static constexpr double _dtheta_max = 2.0 * tau;

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif