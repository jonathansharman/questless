//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines shorter type aliases for reference and pointer types.

#pragma once

#include <memory>
#include <functional>

namespace ql
{
	template <typename T> using ref = std::reference_wrapper<T>;
	template <typename T> using cref = std::reference_wrapper<T const>;

	template <typename T> using uptr = std::unique_ptr<T>;
	template <typename T> using cuptr = std::unique_ptr<T const>;

	template <typename T> using sptr = std::shared_ptr<T>;
	template <typename T> using csptr = std::shared_ptr<T const>;
}
