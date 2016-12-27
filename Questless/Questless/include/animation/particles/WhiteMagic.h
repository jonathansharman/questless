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
		WhiteMagic(GamePoint position) : Particle
			{ position
			, random_displacement(80.0) / 1.0s
			, GameVector{0.0, 50.0} / 1.0s / 1.0s
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{seconds_f{uniform(2.0, 2.5)}}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(GamePoint position) { return std::make_unique<WhiteMagic>(position); }
	private:
		static constexpr Hertz _vx_percent_drag_rate = 1.8_Hz;
		static constexpr double _dtheta_max = 2.0 * tau;

		void subupdate() override;

		const sdl::Texture& texture() const override;
	};
}

#endif