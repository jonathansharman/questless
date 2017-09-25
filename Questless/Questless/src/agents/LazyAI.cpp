//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/LazyAI.h"

#include "Game.h"

using std::function;

namespace questless
{
	void LazyAI::act() { being.busy_time += uniform(1.0, 2.0); }
}
