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
		BlueMagic(units::GamePoint position) : Particle
			{ position
			, units::GameVector{units::GameRadians::circle() / 6.0 * uniform(0, 6), 45.0} / 1.0s
			, units::GameAcceleration::zero()
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(2.0, 2.4)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position) { return std::make_unique<BlueMagic>(position); }
	private:
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() override {}

		const sdl::Texture& texture() const override;
	};
}

#endif
