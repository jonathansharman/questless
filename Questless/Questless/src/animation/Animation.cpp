//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/Animation.h"
#include "Game.h"

using namespace sdl;
using namespace units;

namespace questless
{
	GameSeconds Animation::elapsed_time() const
	{
		return Game::frame_duration * time_scale;
	}
}
