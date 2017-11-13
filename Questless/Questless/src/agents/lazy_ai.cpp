//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/lazy_ai.hpp"

#include "game.hpp"
#include "utility/random.hpp"

namespace ql
{
	void lazy_ai::act() { being.busy_time += uniform(1.0, 2.0); }
}
