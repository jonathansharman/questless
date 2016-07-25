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
		/// Constructs a red magic particle with the specified position.
		/// @param position The particle's starting position.
		RedMagic(const PointF& position) : Particle
			{ position
			, Velocity{random_displacement(_v_max)} + Velocity{0.0, _vyi_adjustment}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-_dtheta_max, _dtheta_max)}
			, 1.0
			, 0.0_Hz
			, double_seconds{uniform(_lifetime_min, _lifetime_max)}
			}
		{}
	private:
		static constexpr double _v_max = 200.0;
		static constexpr double _vyi_adjustment = -150.0;
		static constexpr double _dtheta_max = 720.0;
		static constexpr Hertz _gravity = 300.0_Hz;
		static constexpr double _lifetime_min = 0.6;
		static constexpr double _lifetime_max = 1.0;

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif