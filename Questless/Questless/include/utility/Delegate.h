/**
* @file    Delegate.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Delegate class, which holds a modifiable list of callback functions that can be invoked in order.
*/

#ifndef DELEGATE_H
#define DELEGATE_H

#include <vector>
#include <functional>
#include <memory>

namespace questless
{
	template <typename... Args>
	class Delegate
	{
	public:
		/// The delegate callback type.
		using callback_t = std::shared_ptr<std::function<void(Args...)>>;

		/// Adds a new callback function at the end of the callbacks list.
		/// @param f The new callback.
		void add(const callback_t& f) { _callbacks.push_back(f); }

		/// Removes any instances of a given callback function.
		/// @param f The callback to be removed.
		void remove(const callback_t& f)
		{
			_callbacks.erase(remove_if(_callbacks.begin(), _callbacks.end(), [&f](const callback_t& x) {
				return x == f;
			}), _callbacks.end());
		}

		Delegate& operator +=(const callback_t& f) { add(f); }
		Delegate& operator -=(const callback_t& f)
		{
			remove(f);
			return *this;
		}

		/// Calls each callback in turn, passing them the given arguments.
		/// @param args The callback arguments.
		void operator ()(Args... args)
		{
			for (auto& callback : _callbacks) {
				(*callback)(args...);
			}
		}
	private:
		std::vector<callback_t> _callbacks; ///< The list of callbacks, in the order they will be called.
	};
}

#endif