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

namespace questless
{
	class YellowMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		YellowMagic(units::GamePoint position) : Particle
			{ position
			, units::GameVector{random_angle(), 100.0} / 1.0s
			, units::GameAcceleration::zero()
			, random_angle()
			, units::GameRadians{0.0} / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity::zero()
			, Lifetime{units::GameSeconds{uniform(0.8, 1.2)}}
			, MaxDisplacement{30.0}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position) { return std::make_unique<YellowMagic>(position); }
	private:
		static constexpr units::GameRadians _max_turn_rate = units::GameRadians::circle() / 6.0;

		void subupdate() override;

		const sdl::Texture& texture() const override;
	};
}

#endif
