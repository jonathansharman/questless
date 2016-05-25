/**
* @file    Event.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Event class, which holds a modifiable list of handler callbacks that can be invoked in order.
*          Differs from Delegate in that handlers can mark the event as handled, in which case later callbacks are not invoked.
*/

#ifndef EVENT_H
#define EVENT_H

#include <vector>

namespace questless
{
	template <typename... Args>
	class Event
	{
	public:
		/// The event handler type.
		using handler_t = std::shared_ptr<std::function<bool(Args...)>>;

		/// Adds a new event handler at the end of the handlers list.
		/// @param f The new event handler.
		void add(const handler_t& f) { _handlers.push_back(f); }

		/// Removes any instances of a given event handler.
		/// @param f The event handler to be removed.
		void remove(const handler_t& f)
		{
			_handlers.erase(remove_if(_handlers.begin(), _handlers.end(), [&f](const handler_t& x) {
				return x == f;
			}), _handlers.end());
		}

		Event& operator +=(const handler_t& f) { add(f); }
		Event& operator -=(const handler_t& f)
		{
			remove(f);
			return *this;
		}

		/// Calls each event handler in turn, passing them the given arguments.
		/// @param args The event handler arguments.
		void operator ()(Args... args)
		{
			for (auto& handler : _handlers) {
				if (!(*handler)(args...)) {
					return;
				}
			}
		}
	private:
		std::vector<handler_t> _handlers; ///< The list of callbacks, in the order they will be called.
	};
}

#endif
