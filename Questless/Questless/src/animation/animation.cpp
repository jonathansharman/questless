//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/animation.h"
#include "game.h"

using namespace sdl;
using namespace units;

namespace ql
{
	game_space::seconds animation::elapsed_time() const
	{
		return game::frame_duration * time_scale.get();
	}
}
