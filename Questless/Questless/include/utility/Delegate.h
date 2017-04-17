//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace questless
{
	//! A modifiable list of callback functions that can be invoked in order.
	template <typename... Args>
	class Delegate
	{
	public:
		//! The delegate callback type.
		using callback_t = std::shared_ptr<std::function<void(Args...)>>; //! @todo This shared_ptr and the one in Event are smelly.

		//! Adds a new callback function @p f at the end of the callbacks list.
		void add(callback_t const& f) { _callbacks.push_back(f); }

		//! Removes any instances of the callback function @p f.
		void remove(callback_t const& f)
		{
			_callbacks.erase(remove_if(_callbacks.begin(), _callbacks.end(), [&f](callback_t const& x) {
				return x == f;
			}), _callbacks.end());
		}

		Delegate& operator +=(callback_t const& f) { add(f); }
		Delegate& operator -=(callback_t const& f)
		{
			remove(f);
			return *this;
		}

		//! Calls each callback in turn, passing them @p args.
		void operator ()(Args... args)
		{
			for (auto& callback : _callbacks) {
				(*callback)(args...);
			}
		}
	private:
		std::vector<callback_t> _callbacks; //!< The list of callbacks, in the order they will be called.
	};
}
