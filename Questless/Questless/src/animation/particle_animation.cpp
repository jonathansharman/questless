//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "particle_animation.hpp"

namespace ql {
	void particle_animation::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto& particle : particles) {
			particle->draw(target, states);
		}
	}

	void particle_animation::animation_subupdate(sec elapsed_time) {
		for (auto it = particles.begin(); it != particles.end();) {
			(*it)->update(elapsed_time);
			if ((*it)->stopped()) {
				it = particles.erase(it);
			} else {
				++it;
			}
		}

		particle_animation_subupdate(elapsed_time);
	}
}
