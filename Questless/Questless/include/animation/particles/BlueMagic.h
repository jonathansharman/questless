/**
* @file    BlueMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BlueMagic particle class.
*/

#ifndef BLUE_MAGIC_H
#define BLUE_MAGIC_H

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class BlueMagic : public Particle
	{
	public:
		/// Constructs a blue magic particle with the specified position.
		/// @param position The particle's starting position.
		BlueMagic(const PointF& position) : Particle
			{ position
			, Velocity{ParticleK::blue_v, 0.0}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-ParticleK::blue_dtheta_max, ParticleK::blue_dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(ParticleK::blue_lifetime_min, ParticleK::blue_lifetime_max)}
			}
		{
			_velocity.rotate(60.0 * uniform(0, 6));
		}

		/// Updates the particle.
		void update() override;
	private:
		/// @return The texture to be used when drawing a particle.
		Texture::ptr& texture() const override;
	};
}

#endif