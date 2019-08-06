//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Substitutes for select Concurrency TS functionality, while awaiting adoption into the standard.

#pragma once

#include <future>

namespace ql {
	//! Makes a @p std::future from @p value that is immediately ready.
	template <typename T>
	std::future<T> make_ready_future(T value) {
		std::promise<T> promise;
		promise.set_value(std::move(value));
		return promise.get_future();
	}

	//! Makes a @p std::future<void> that is immediately ready.
	inline std::future<void> make_ready_future() {
		std::promise<void> promise;
		promise.set_value();
		return promise.get_future();
	}
}
