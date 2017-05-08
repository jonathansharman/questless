//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/FlameParticle.h"
#include "Game.h"
#include "sdl/resources.h"

using namespace sdl;

namespace questless
{
	void FlameParticle::subupdate()
	{
		_velocity.step().x *= 1.0 - _vx_percent_drag_rate * Game::frame_duration;
	}

	Texture const& FlameParticle::texture() const
	{
		static auto texture_handle = texture_manager().add("resources/textures/particles/glow-small.png");
		texture_manager()[texture_handle].color(Color{255, static_cast<uint8_t>(uniform(0, 255)), 0});
		return texture_manager()[texture_handle];
	}
}
