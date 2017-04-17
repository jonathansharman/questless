//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/WhiteMagicParticle.h"
#include "Game.h"
#include "sdl/resources.h"

namespace questless
{
	void WhiteMagicParticle::subupdate()
	{
		_velocity.step().x *= 1.0 - _vx_percent_drag_rate * Game::frame_duration;
	}

	sdl::Texture const& WhiteMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/white.png");
		return sdl::texture_manager()[texture_handle];
	}
}
