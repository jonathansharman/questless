//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/ParticleAnimation.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void ParticleAnimation::draw(units::ScreenSpace::Point position) const
	{
		for (auto& particle : _particles) {
			particle->draw(position);
		}
	}

	void ParticleAnimation::draw(units::GameSpace::Point position, Camera const& camera, colors::ColorFactor color_factor) const
	{
		for (auto& particle : _particles) {
			particle->draw(position, camera, color_factor);
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
