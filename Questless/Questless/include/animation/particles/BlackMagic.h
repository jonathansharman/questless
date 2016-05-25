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

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	class BlackMagic : public Particle
	{
	public:
		/// Constructs a black magic particle with the specified position.
		/// @param position The particle's starting position.
		BlackMagic(const PointF& position) : Particle
			{ position
			, Velocity{uniform(ParticleK::black_vxi_min, ParticleK::black_vxi_max), 0.0}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-ParticleK::black_dtheta_max, ParticleK::black_dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{2.0}
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