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

#include <memory>
using std::unique_ptr;
using std::make_unique;

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
			, Velocity{random_displacement(ParticleK::red_v_max)} +Velocity{0.0, ParticleK::red_vyi_adjustment}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-ParticleK::red_dtheta_max, ParticleK::red_dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(ParticleK::red_lifetime_min, ParticleK::red_lifetime_max)}
			}
		{}

		/// Updates the particle.
		void update() override;
	private:
		/// @return The texture to be used when drawing a particle.
		Texture::ptr& texture() const override;
	};
}

#endif