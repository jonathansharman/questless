/**
* @file    Event.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Event class, which holds a modifiable list of handler callbacks that can be invoked in order.
*          Differs from Delegate in that handlers may return false to indicate not to continue invoking callbacks.
*/

#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace questless
{
	template <typename... Args>
	class Event
	{
	public:
		/// The event handler type. Return type indicates whether the event was handled.
		using handler_t = std::shared_ptr<std::function<bool(Args...)>>; /// @todo This shared_ptr and the one in Delegate are smelly.

		/// Adds a new event handler at the end of the handlers list.
		/// @param f The new event handler.
		void add(handler_t const& f) { _handlers.push_back(f); }

		/// Removes any instances of a given event handler.
		/// @param f The event handler to be removed.
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

		/// Calls each event handler in turn, passing them the given arguments.
		/// @param args The event handler arguments.
		/// @return True if no callback marked the event as handled, false otherwise.
		/*[[nodiscard]]*/ bool operator ()(Args... args) /// @todo Uncomment [[nodiscard]] once supported.
		{
			for (auto& handler : _handlers) {
				if (!(*handler)(args...)) {
					return false;
				}
			}
			return true;
		}
	private:
		std::vector<handler_t> _handlers; ///< The list of callbacks, in the order they will be called.
	};
}
