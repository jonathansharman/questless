/**
* @file    GreenMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the GreenMagic particle class.
*/

#ifndef GREEN_MAGIC_H
#define GREEN_MAGIC_H

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class GreenMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		GreenMagic(const PointF& position)
			: Particle
				{ position
				, Velocity{random_displacement(_v_min, _v_max)}
				, ANGLE_DEGREES = uniform(0.0, 360.0)
				, ANGULAR_VELOCITY = Hertz{uniform(-_dtheta_max, _dtheta_max)}
				, SCALE = 1.0
				, SCALE_VELOCITY = 0.0_Hz
				, LIFETIME = seconds_f{uniform(_lifetime_min, _lifetime_max)}
				}
			, _turning_right{random_bool()}
		{}
	private:
		static constexpr double _dtheta_max = 720.0;
		static constexpr double _v_min = 20.0;
		static constexpr double _v_max = 50.0;
		static constexpr double _inflection_probability = 0.1;
		static constexpr double _turn_rate = 5.0;
		static constexpr double _lifetime_min = 1.8;
		static constexpr double _lifetime_max = 2.2;

		bool _turning_right; ///< True if the green magic particle is turning to the right (CW), false if turning left (CCW).

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif