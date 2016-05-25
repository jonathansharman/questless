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

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class GreenMagic : public Particle
	{
	public:
		/// Constructs a white magic particle with the specified position.
		/// @param position The particle's starting position.
		GreenMagic(const PointF& position) : Particle
			{ position
			, Velocity{random_displacement(ParticleK::green_v_min, ParticleK::green_v_max)}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-ParticleK::green_dtheta_max, ParticleK::green_dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(ParticleK::green_lifetime_min, ParticleK::green_lifetime_max)}
			}
			, _turning_right{random_bool()}
		{}

		/// Updates the particle.
		void update() override;
	private:
		/// @return The texture to be used when drawing a particle.
		Texture::ptr& texture() const override;

		bool _turning_right; ///< True if the green magic particle is turning to the right (CW), false if turning left (CCW).
	};
}

#endif