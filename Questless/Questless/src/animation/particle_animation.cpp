//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "particle_animation.hpp"

#include <range/v3/action/remove_if.hpp>

namespace ql {
	void particle_animation::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& particle : particles) {
			target.draw(*particle, states);
		}
	}

	void particle_animation::animation_subupdate(sec elapsed_time) {
		// Update each particle.
		for (auto& particle : particles) {
			particle->update(elapsed_time);
		}
		// Remove stopped particles.
		ranges::actions::remove_if(particles, [](auto const& particle) { return particle->stopped(); });

		particle_animation_subupdate(elapsed_time);
	}
}
