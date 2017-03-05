/**
* @file    GreenMagicParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Green magic particle implementation.
*/

#include "animation/particles/GreenMagicParticle.h"
#include "Game.h"
#include "sdl/resources.h"

namespace questless
{
	void GreenMagicParticle::subupdate()
	{
		if (uniform(0.0, 1.0) < _inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.step().rotate(units::GameRadians{-1.0 * _turn_rate * Game::frame_duration});
		} else {
			_velocity.step().rotate(units::GameRadians{_turn_rate * Game::frame_duration});
		}
	}

	sdl::Texture const& GreenMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/green.png");
		return sdl::texture_manager()[texture_handle];
	}
}
