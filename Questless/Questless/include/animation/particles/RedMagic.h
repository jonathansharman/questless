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
		RedMagic(units::GamePoint position) : Particle
			{ position
			, (random_displacement(200.0) + units::GameVector{0.0, 150.0}) / 1.0s
			, units::GameVector{0.0, -300.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(0.6, 1.0)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position) { return std::make_unique<RedMagic>(position); }
	private:
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif
