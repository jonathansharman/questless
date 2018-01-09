//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/flame_particle.hpp"
#include "game.hpp"
#include "sdl/resources.hpp"

using namespace sdl;

namespace ql
{
	void flame_particle::particle_subupdate()
	{
		_velocity.step().x() *= 1.0 - _vx_pct_drag_rate * game::frame_duration;

		float pct_left = static_cast<float>(_time_left.count() / _lifetime.count());

		// Add a random "flicker".
		pct_left = std::clamp(pct_left + uniform(-0.3f, 0.3f), 0.0f, 1.0f);

		// Fade from yellow to red to black.
		if (pct_left > 0.5) {
			_color_vector.green() = 2.0f * (pct_left - 0.5f);
		} else {
			_color_vector.red() = 2.0f * pct_left;
			_color_vector.green() = 0.0f;
		}
	}

	texture const& flame_particle::texture() const
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/particles/glow-small.png");
		return the_texture_manager()[texture_handle];
	}
}
