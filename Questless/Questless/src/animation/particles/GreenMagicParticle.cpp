//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/GreenMagicParticle.h"
#include "Game.h"
#include "sdl/resources.h"

namespace questless
{
	void GreenMagicParticle::particle_subupdate()
	{
		if (uniform(0.0, 1.0) < _inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.step().rotate(units::GameSpace::Radians{-1.0 * _turn_rate * Game::frame_duration});
		} else {
			_velocity.step().rotate(units::GameSpace::Radians{_turn_rate * Game::frame_duration});
		}
	}

	sdl::Texture const& GreenMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/green.png");
		return sdl::texture_manager()[texture_handle];
	}
}
