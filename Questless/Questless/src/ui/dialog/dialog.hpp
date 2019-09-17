//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "ui/widget.hpp"

#include <future>
#include <optional>

namespace ql {
	//! A widget used to retrieve a single data item from the player.
	template <typename T>
	struct dialog : widget {
		auto get_future() -> std::future<std::optional<T>> {
			return _promise.get_future();
		}

	protected:
		std::promise<std::optional<T>> _promise;
	};
}
