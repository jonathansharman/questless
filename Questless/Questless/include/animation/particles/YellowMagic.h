/**
* @file    YellowMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the YellowMagic particle class.
*/

#ifndef YELLOW_MAGIC_H
#define YELLOW_MAGIC_H

#include <memory>

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class YellowMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		YellowMagic(PointF position) : Particle
			{ position
			, VectorF{random_angle(), LengthSquared{100.0}} / 1.0s
			, Acceleration::zero()
			, random_angle()
			, AngularVelocity{0.0}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(0.8, 1.2)}}
			, MaxDisplacement{30.0}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<YellowMagic>(position); }
	private:
		static constexpr double _turn_rate_min = -60.0;
		static constexpr double _turn_rate_max = 60.0;

		void subupdate() override;

		const sdl::Texture& texture() const override;
	};
}

#endif
