//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/FlameParticle.h"
#include "Game.h"
#include "sdl/resources.h"

using namespace sdl;

namespace questless
{
	void FlameParticle::particle_subupdate()
	{
		_velocity.step().x *= 1.0 - _vx_percent_drag_rate * Game::frame_duration;

		//_color = sdl::Color{255, static_cast<uint8_t>(uniform(0, 255)), 0};

		double percent_left = _time_left.count() / _lifetime.count();

		// Add a random "flicker".
		percent_left = std::clamp(percent_left + uniform(-0.3, 0.3), 0.0, 1.0);

		// Fade from yellow to red to black.
		if (percent_left > 0.5) {
			_color.g = percentage_to_byte(2.0 * (percent_left - 0.5));
		} else {
			_color.g = 0;
			_color.r = percentage_to_byte(2.0 * percent_left);
		}
	}

	Texture const& FlameParticle::texture() const
	{
		static auto texture_handle = texture_manager().add("resources/textures/particles/glow-small.png");
		return texture_manager()[texture_handle];
	}
}
