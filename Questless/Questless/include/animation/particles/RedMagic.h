/**
* @file    RedMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the RedMagic particle class.
*/

#ifndef RED_MAGIC_H
#define RED_MAGIC_H

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class RedMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		RedMagic(PointF position) : Particle
			{ position
			, (random_displacement(200.0) + VectorF{0.0, -150.0}) / 1.0s
			, VectorF{0.0, 300.0} / 1.0s / 1.0s
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(0.6, 1.0)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<RedMagic>(position); }
	private:
		static constexpr double _dtheta_max = 720.0;

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif
