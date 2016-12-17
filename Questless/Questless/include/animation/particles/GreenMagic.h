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
		GreenMagic(PointF position)
			: Particle
				{ position
				, Velocity{random_displacement(_v_min, _v_max)}
				, random_angle()
				, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
				, Scale(1.0)
				, ScaleVelocity{0.0}
				, Lifetime{seconds_f{uniform(_lifetime_min, _lifetime_max)}}
				}
			, _turning_right{random_bool()}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<GreenMagic>(position); }
	private:
		static constexpr double _dtheta_max = 2.0 * tau;
		static constexpr double _v_min = 20.0;
		static constexpr double _v_max = 50.0;
		static constexpr double _inflection_probability = 0.1;
		static constexpr AngularVelocity _turn_rate{tau};
		static constexpr double _lifetime_min = 1.8;
		static constexpr double _lifetime_max = 2.2;

		bool _turning_right; ///< True if the green magic particle is turning to the right (CW), false if turning left (CCW).

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif
