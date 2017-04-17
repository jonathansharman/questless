//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <chrono>

namespace units
{
	using GameSeconds = std::chrono::duration<double>;
}

// Bring chrono_literals into the namespace, for convenience when dealing with game time.

using namespace std::literals::chrono_literals;
