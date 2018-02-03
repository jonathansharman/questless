//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"
#include "particles/blood_particle.hpp"

namespace ql
{
	//! Creates drops of blood.
	class bleeding : public particle_animation
	{
	public:
		//! @param drops_per_update The number of drops of blood to create per frame. Can be less than one.
		bleeding(double drops_per_update) : _drops_per_update(drops_per_update) {}
	private:
		double _drops_per_update;
		double _drops = 0.0;

		void particle_animation_subupdate() final
		{
			_drops += _drops_per_update;
			while (_drops > 1.0) {
				push_front(umake<blood_particle>());
				--_drops;
			}
		}
	};
}
