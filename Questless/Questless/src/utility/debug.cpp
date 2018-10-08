//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "utility/debug.hpp"
#include "game.hpp"

namespace ql {
	std::chrono::nanoseconds time(std::function<void()> const& f) {
		auto start_time = game::clock::now();
		f();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(game::clock::now() - start_time);
	}
}
