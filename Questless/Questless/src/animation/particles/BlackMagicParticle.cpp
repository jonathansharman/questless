//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/BlackMagicParticle.h"
#include "Game.h"
#include "sdl/resources.h"
using namespace units;

namespace questless
{
	void BlackMagicParticle::subupdate()
	{
		_velocity *= 1.0 + _acceleration_factor * Game::frame_duration;
		_velocity.step().rotate(GameRadians{_turn_rate * Game::frame_duration});
	}

	sdl::Texture const& BlackMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/black.png");
		return sdl::texture_manager()[texture_handle];
	}
}
