//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "particle.hpp"

#include "ui/view_space.hpp"
#include "utility/random.hpp"

namespace ql {
	particle::particle(sec lifetime) : lifetime{lifetime} {}

	auto particle::animation_subupdate(sec elapsed_time) -> void {
		// Timing.
		time_left -= elapsed_time;
		if (time_left <= 0.0_s) {
			stop();
			return;
		}

		// Update physics.
		displacement += velocity * elapsed_time;
		velocity += acceleration * elapsed_time;
		if (face_towards_heading() && velocity != view::vel::zero()) {
			angle = velocity.angle();
		} else {
			angle += angular_velocity * elapsed_time;
		}
		scale += scale_velocity * elapsed_time;

		// Update alpha, if fading out is enabled.
		if (fade_out()) { color_factor.a = to_uint8(cancel::ratio<float>(time_left, lifetime)); }

		// Subupdate.
		particle_subupdate(elapsed_time);
	}

	auto particle::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		// Combine animation/other transforms with particle transforms.
		states.transform.scale({scale.value, scale.value});
		states.transform.rotate(static_cast<float>((angle * vecx::deg_per_rad).value));
		states.transform.translate(to_sfml(displacement));

		// Subdraw.
		particle_subdraw(target, states);
	}
}
