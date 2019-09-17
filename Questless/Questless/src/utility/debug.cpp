//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "debug.hpp"

#include "quantities/wall_time.hpp"

namespace ql {
	auto time(std::function<void()> const& f) -> std::chrono::nanoseconds {
		auto start_time = clock::now();
		f();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - start_time);
	}
}
