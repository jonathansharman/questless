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
		/// Constructs a yellow magic particle with the specified position.
		/// @param position The particle's starting position.
		YellowMagic(const PointF& position) : Particle
			{ position + random_displacement(ParticleK::max_displacement)
			, Velocity{ParticleK::yellow_v, 0.0}
			, uniform(0.0, 360.0)
			, 0.0
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(ParticleK::yellow_lifetime_min, ParticleK::yellow_lifetime_max)}
			}
		{
			_velocity.rotate(uniform(0.0, 360.0));
		}

		/// Updates the particle.
		void update() override;
	private:
		/// @return The texture to be used when drawing a particle.
		Texture::ptr& texture() const override;
	};
}

#endif
