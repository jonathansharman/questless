//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/ParticleAnimation.h"

using namespace units;
using namespace sdl;

namespace questless
{
	void ParticleAnimation::draw(units::ScreenPoint position) const
	{
		//! @todo Support this.
		throw std::logic_error{"Particle animation does not support direct-to-screen drawing."};
	}

	void ParticleAnimation::draw(units::GamePoint position, Camera const& camera, sdl::Color color) const
	{
		for (auto& particle : _particles) {
			particle->draw(camera);
		}
	}

	void ParticleAnimation::animation_subupdate()
	{
		for (std::size_t i = 0; i < _particles.size();) {
			_particles[i]->update();
			if (_particles[i]->expired()) {
				_particles.erase(_particles.begin() + i);
			} else {
				++i;
			}
		}

		particle_animation_subupdate();
	}
}
