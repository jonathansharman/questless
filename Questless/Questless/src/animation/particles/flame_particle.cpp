//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "flame_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

using namespace vecx;
using namespace vecx::literals;

namespace ql {
	using namespace view::literals;

	flame_particle::flame_particle(rsrc::particle const& resources)
		: sprite_particle{uniform(1.0_s, 1.5_s), resources.glow_small} //
	{
		displacement = random_displacement(5.0_px);
		acceleration = view::vector{0.0_px, 30.0_px} / 1.0_s / 1.0_s;
		scale = {0.75};
		color_factor = sf::Color{255, 128, 0};
	}

	void flame_particle::sprite_particle_subupdate(sec elapsed_time) {
		static constexpr auto vx_pct_drag_rate = 1.8_hz;

		velocity[0] -= velocity[0] * vx_pct_drag_rate * elapsed_time;

		auto pct_left = static_cast<float>((time_left / lifetime).value);

		// Add a random "flicker".
		pct_left = std::clamp(pct_left + uniform(-0.3f, 0.3f), 0.0f, 1.0f);

		// Fade from yellow to red to black.
		if (pct_left > 0.5f) {
			color_factor.g = to_uint8(2.0f * (pct_left - 0.5f));
		} else {
			color_factor.r = to_uint8(2.0f * pct_left);
			color_factor.g = 0;
		}
	}
}
