//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "flame_particle.hpp"

#include "game.hpp"
#include "utility/random.hpp"

using namespace ql::world::literals;

using namespace media;
using namespace vecx;
using namespace vecx::literals;

namespace ql {
	namespace {
		sf::Texture const& texture() {
			static auto texture_handle = the_texture_manager().add("resources/textures/particles/glow-small.png");
			return the_texture_manager()[texture_handle];
		}
	}

	flame_particle::flame_particle() : sprite_particle{texture(), uniform(1.0_s, 1.5_s)} {
		displacement = random_displacement(5.0_world_length);
		acceleration = world::vector{0.0_world_length, 30.0_world_length} / 1.0_s / 1.0_s;
		scale = {0.75};
		color_factor = sf::Color{255, 128, 0};
	}
	void flame_particle::sprite_particle_subupdate(sec elapsed_time) {
		static constexpr auto _vx_pct_drag_rate = 1.8_hz;

		velocity[0] *= cancel::unitless<double>{1.0} - _vx_pct_drag_rate * elapsed_time;

		float pct_left = (time_left / lifetime).value;

		// Add a random "flicker".
		pct_left = std::clamp(pct_left + uniform(-0.3f, 0.3f), 0.0f, 1.0f);

		// Fade from yellow to red to black.
		if (pct_left > 0.5) {
			color_factor.g = to_uint8(2.0f * (pct_left - 0.5f));
		} else {
			color_factor.r = to_uint8(2.0f * pct_left);
			color_factor.g = 0;
		}
	}
}
