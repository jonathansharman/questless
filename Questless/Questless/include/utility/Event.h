//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace questless
{
	//! Holds a modifiable list of handler callbacks that can be invoked in order.
	//!
	//! Differs from Delegate in that handlers may return false to indicate not to continue invoking callbacks.
	template <typename... Args>
	class Event
	{
	public:
		//! The event handler type. Return type indicates whether the event was handled.
		using handler_t = std::shared_ptr<std::function<bool(Args...)>>; //! @todo This shared_ptr and the one in Delegate are smelly.

		//! Adds a new event handler @p f at the end of the handlers list.
		void add(handler_t const& f) { _handlers.push_back(f); }

		//! Removes any instances of the event handler @p f.
		void remove(handler_t const& f)
		{
			_handlers.erase(remove_if(_handlers.begin(), _handlers.end(), [&f](handler_t const& x) {
				return x == f;
			}), _handlers.end());
		}

		Event& operator +=(handler_t const& f) { add(f); }
		Event& operator -=(handler_t const& f)
		{
			remove(f);
			return *this;
		}

		//! Calls each event handler in turn, passing them @p args.
		//! @return True if no callback marked the event as handled, false otherwise.
		/*[[nodiscard]]*/ bool operator ()(Args... args) //! @todo Uncomment [[nodiscard]] once supported.
		{
			for (auto& handler : _handlers) {
				if (!(*handler)(args...)) {
					return false;
				}
			}
			return true;
		}
	private:
		std::vector<handler_t> _handlers; //!< The list of callbacks, in the order they will be called.
	};
}
