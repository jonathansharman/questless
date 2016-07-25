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
		/// Constructs a white magic particle with the specified position.
		/// @param position The particle's starting position.
		WhiteMagic(const PointF& position) : Particle
			{ position
			, Velocity{random_displacement(_vi_max)}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-_dtheta_max, _dtheta_max)}
			, 1.0
			, 0.0_Hz
			, double_seconds{uniform(_lifetime_min, _lifetime_max)}
			}
		{}
	private:
		static constexpr Hertz _gravity{-50.0};
		static constexpr double _vi_max = 80.0;
		static constexpr double _vx_factor_per_frame = 0.97;
		static constexpr double _dtheta_max = 720.0;
		static constexpr double _lifetime_min = 2.00;
		static constexpr double _lifetime_max = 2.50;

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif