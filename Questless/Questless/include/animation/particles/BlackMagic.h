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
		BlackMagic(PointF position) : Particle
			{ position
			, Velocity{VectorF{random_angle(), Length{uniform(5.0, 25.0)}}}
			, Acceleration::zero()
			, random_angle()
			, AngularVelocity{uniform(-_dtheta_max, _dtheta_max)}
			, Scale{1.0}
			, ScaleVelocity{0.0}
			, Lifetime{2.0s}
			}
		{}

		/// @param position The particle's starting position.
		static ptr make(PointF position) { return std::make_unique<BlackMagic>(position); }
	private:
		static constexpr AngularVelocity _turn_rate{4.0};
		static constexpr double _dtheta_max = 2.0 * tau;
		static constexpr Hertz _acceleration_factor = 1.25_Hz;

		void subupdate() override;

		const sdl::Texture& texture() const override;
	};
}

#endif
