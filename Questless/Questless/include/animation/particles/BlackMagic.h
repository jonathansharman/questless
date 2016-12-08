/**
* @file    BlackMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BlackMagic particle class.
*/

#ifndef BLACK_MAGIC_H
#define BLACK_MAGIC_H

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class BlackMagic : public Particle
	{
	public:
		/// @param position The particle's starting position.
		BlackMagic(const PointF& position) : Particle
			{ position
			, Velocity{uniform(_vxi_min, _vxi_max), 0.0}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-_dtheta_max, _dtheta_max)}
			, 1.0
			, 0.0_Hz
			, 2.0s
			}
		{
			_velocity.rotate(uniform(0.0, 360.0));
		}
	private:
		static constexpr Hertz _turn_rate = 225.0_Hz;
		static constexpr double _vxi_min = 5.0;
		static constexpr double _vxi_max = 25.0;
		static constexpr double _dtheta_max = 720.0;
		static constexpr Hertz _acceleration_factor = 1.25_Hz;
		static constexpr seconds_f _lifetime = 2.5s;

		void subupdate() override;

		sdl::Texture& texture() const override;
	};
}

#endif
