//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "flame_particle.hpp"

#include "game.hpp"
#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	flame_particle::flame_particle() : particle
		{ world_space::vector::zero()
		, world_space::velocity::zero()
		, world_space::vector{0.0, 30.0} / 1.0s / 1.0s
		, world_space::radians::zero()
		, world_space::radians_per_sec::zero()
		, 0.75
		, world_space::scale_velocity::zero()
		, world_space::seconds{uniform(1.0, 1.5)}
		, max_displacement{5.0}
		}
	{
		_color_vector = colors::orange_vector();
	}
	void flame_particle::particle_subupdate() {
		_velocity.step().x() *= 1.0 - _vx_pct_drag_rate * target_frame_duration;

		float pct_left = (_time_left / _lifetime).value;

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

	texture const& flame_particle::texture() const {
		static auto texture_handle = the_texture_manager().add("resources/textures/particles/glow-small.png");
		return the_texture_manager()[texture_handle];
	}
}
