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
		WhiteMagic(units::GamePoint position) : Particle
			{ position
			, random_displacement(80.0) / 1.0s
			, units::GameVector{0.0, 50.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(2.0, 2.5)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position) { return std::make_unique<WhiteMagic>(position); }
	private:
		static constexpr units::Hertz _vx_percent_drag_rate = 1.8_Hz;
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() override;

		const sdl::Texture& texture() const override;
	};
}

#endif