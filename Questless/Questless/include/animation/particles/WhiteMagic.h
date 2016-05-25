/**
* @file    WhiteMagic.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WhiteMagic particle class.
*/

#ifndef WHITE_MAGIC_H
#define WHITE_MAGIC_H

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class WhiteMagic : public Particle
	{
	public:
		/// Constructs a white magic particle with the specified position.
		/// @param position The particle's starting position.
		WhiteMagic(const PointF& position) : Particle
			{ position
			, Velocity{random_displacement(ParticleK::white_vi_max)}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-ParticleK::white_dtheta_max, ParticleK::white_dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(ParticleK::white_lifetime_min, ParticleK::white_lifetime_max)}
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