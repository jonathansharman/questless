//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "particle.hpp"

#include "utility/random.hpp"
#include "world/world_space.hpp"

namespace ql {
	particle::particle(sec lifetime) : lifetime{lifetime} {}

	void particle::animation_subupdate(sec elapsed_time) {
		// Timing.
		time_left -= elapsed_time;
		if (time_left <= 0.0_s) {
			stop();
			return;
		}

		// Update physics.
		displacement += velocity * elapsed_time;
		velocity += acceleration * elapsed_time;
		if (face_towards_heading() && velocity != world::vel::zero()) {
			angle = velocity.angle();
		} else {
			angle += angular_velocity * elapsed_time;
		}
		scale += scale_velocity * elapsed_time;

		// Update alpha, if fading out is enabled.
		if (fade_out()) { color_factor.a = static_cast<sf::Uint8>(255 * (time_left / lifetime).value); }

		// Subupdate.
		particle_subupdate(elapsed_time);
	}

	void particle::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Combine animation/other transforms with particle transforms.
		states.transform.scale({static_cast<float>(scale.value), static_cast<float>(scale.value)});
		states.transform.rotate(static_cast<float>((angle * vecx::deg_per_rad).value));
		states.transform.translate(to_sfml(displacement));

		// Subdraw.
		particle_subdraw(target, states);
	}
}
