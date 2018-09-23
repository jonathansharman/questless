//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Utility functions for debugging.

#pragma once

#include <type_safe/reference.hpp>

#include <chrono>
#include <functional>

namespace ql {
	//! Executes and times the function @p f.
	//! @return The time it took to execute @p f, in nanoseconds.
	std::chrono::nanoseconds time(type_safe::function_ref<void()> f);
}
