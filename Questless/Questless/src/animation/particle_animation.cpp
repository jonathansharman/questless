//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particle_animation.hpp"

using namespace sdl;

namespace ql {
	void particle_animation::draw(spaces::window::point position) const {
		for (auto& particle : _particles) {
			particle->draw(position);
		}
	}

	void particle_animation::draw(world::point position, camera const& camera, sdl::spaces::colors::color color_factor) const {
		for (auto& particle : _particles) {
			particle->draw(position, camera, color_factor);
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
