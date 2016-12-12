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
#include "utility/utility.h"

namespace questless
{
	class YellowMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		YellowMagic(const PointF& position) : Particle
			{ position + random_displacement(_max_displacement)
			, Velocity{_v, 0.0}
			, ANGLE_DEGREES = uniform(0.0, 360.0)
			, ANGULAR_VELOCITY = 0.0
			, SCALE = 1.0
			, SCALE_VELOCITY = 0.0_Hz
			, LIFETIME = seconds_f{uniform(_lifetime_min, _lifetime_max)}
			}
		{
			_velocity.rotate(uniform(0.0, 360.0));
		}

		/// @param position The particle's starting position.
		static ptr make(const PointF& position) { return std::make_unique<YellowMagic>(position); }
	private:
		static constexpr double _max_displacement = 15.0;
		static constexpr double _v = 100.0;
		static constexpr double _turn_rate_min = -60.0;
		static constexpr double _turn_rate_max = 60.0;
		static constexpr double _lifetime_min = 0.8;
		static constexpr double _lifetime_max = 1.2;

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif
