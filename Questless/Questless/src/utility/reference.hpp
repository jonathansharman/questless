//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines shorter type aliases for reference and pointer types.

#pragma once

#include <functional>
#include <memory>

namespace ql {
	template <typename T>
	using ref = std::reference_wrapper<T>;

	template <typename T>
	using uptr = std::unique_ptr<T>;

	template <typename T>
	using sptr = std::shared_ptr<T>;

	//! Shorthand for std::make_unique<T>().
	template <typename T, typename... Args>
	auto umake(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	//! Shorthand for std::make_shared<T>().
	template <typename T, typename... Args>
	auto smake(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
