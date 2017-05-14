//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/ParticleAnimation.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void ParticleAnimation::draw(units::ScreenPoint position) const
	{
		for (auto& particle : _particles) {
			particle->draw(position);
		}
	}

	void ParticleAnimation::draw(units::GamePoint position, Camera const& camera, sdl::Color color) const
	{
		for (auto& particle : _particles) {
			particle->draw(position, camera, color);
		}
	}

	void ParticleAnimation::animation_subupdate()
	{
		for (std::size_t i = 0; i < _particles.size();) {
			_particles[i]->update();
			if (_particles[i]->over()) {
				_particles.erase(_particles.begin() + i);
			} else {
				++i;
			}
		}

		particle_animation_subupdate();
	}
}
