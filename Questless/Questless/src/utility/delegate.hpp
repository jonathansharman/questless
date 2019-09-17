//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace ql {
	//! A modifiable list of callback functions that can be invoked in order.
	template <typename... Args>
	struct delegate {
		//! The delegate callback type.
		using callback_t = sptr<std::function<void(Args...)>>; //! @todo This shared_ptr and the one in event are smelly.

		//! Adds a new callback function @p f at the end of the callbacks list.
		auto add(callback_t const& f) -> void {
			_callbacks.push_back(f);
		}

		//! Removes any instances of the callback function @p f.
		auto remove(callback_t const& f) -> void {
			_callbacks.erase_if(_callbacks, [&f](callback_t const& x) { return x == f; });
		}

		auto& operator+=(callback_t const& f) {
			add(f);
			return *this;
		}
		auto& operator-=(callback_t const& f) {
			remove(f);
			return *this;
		}

		//! Calls each callback in turn, passing them @p args.
		auto operator()(Args... args) -> void {
			for (auto& callback : _callbacks) {
				(*callback)(args...);
			}
		}

	private:
		std::vector<callback_t> _callbacks; //!< The list of callbacks, in the order they will be called.
	};
}
