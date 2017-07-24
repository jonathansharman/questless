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
		_velocity.step().x() *= 1.0 - _vx_percent_drag_rate * Game::frame_duration;

		float percent_left = static_cast<float>(_time_left.count() / _lifetime.count());

		// Add a random "flicker".
		percent_left = std::clamp(percent_left + uniform(-0.3f, 0.3f), 0.0f, 1.0f);

		// Fade from yellow to red to black.
		if (percent_left > 0.5) {
			_color_factor.green() = 2.0f * (percent_left - 0.5f);
		} else {
			_color_factor.red() = 2.0f * percent_left;
			_color_factor.green() = 0.0f;
		}
	}

	Texture const& FlameParticle::texture() const
	{
		static auto texture_handle = texture_manager().add("resources/textures/particles/glow-small.png");
		return texture_manager()[texture_handle];
	}
}
