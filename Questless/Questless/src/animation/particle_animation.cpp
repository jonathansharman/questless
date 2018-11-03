//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particle_animation.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	void particle_animation::draw(units::window_space::point position) const {
		for (auto& particle : _particles) {
			particle->draw(position);
		}
	}

	void particle_animation::draw(units::world_space::point position, camera const& camera, colors::color_vector color_vector) const {
		for (auto& particle : _particles) {
			particle->draw(position, camera, color_vector);
		}
	}

	void particle_animation::animation_subupdate() {
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
