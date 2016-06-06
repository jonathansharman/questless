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
			, Velocity{_v, 0.0}
			, uniform(0.0, 360.0)
			, Hertz{uniform(-_dtheta_max, _dtheta_max)}
			, 1.0
			, Hertz{0.0}
			, double_seconds{uniform(_lifetime_min, _lifetime_max)}
			}
		{
			_velocity.rotate(60.0 * uniform(0, 6));
		}
	private:
		static constexpr double _v = 45.0;
		static constexpr double _dtheta_max = 720.0;
		static constexpr double _lifetime_min = 2.0;
		static constexpr double _lifetime_max = 2.4;

		void subupdate() override {}

		sdl::Texture::ptr& texture() const override;
	};
}

#endif