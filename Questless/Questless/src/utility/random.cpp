//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "utility/random.hpp"

#include "game.hpp"

namespace ql::detail {
	std::mt19937_64 rng(game::clock::now().time_since_epoch().count());
}
