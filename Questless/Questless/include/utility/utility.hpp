//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Miscellaneous utility functions.

#pragma once

#include "reference.hpp"
#include "units/game_space.hpp"

namespace ql
{
	//! Conditionally removes elements from a container.
	//! @param container An iterable container.
	//! @param predicate A predicate over elements of the container. Elements for which the predicate is true are removed.
	template <typename ContainerType, typename PredicateType>
	void erase_if(ContainerType& container, PredicateType&& predicate)
	{
		for (auto it = container.begin(); it != container.end();) {
			if (std::forward<PredicateType>(predicate)(*it)) {
				it = container.erase(it);
			} else {
				++it;
			}
		}
	};

	//! Moves @p pointer into a new vector and returns it.
	template <typename T, typename U>
	static auto make_uptr_vector(uptr<U> pointer)
	{
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(pointer));
		return modifiers;
	}

	//! Moves the given pointers into a new vector and returns it.
	template <typename T, typename First, typename... Rest>
	static auto make_uptr_vector(First first, Rest... rest)
	{
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(first));
		auto rest = make_uptr_vector<T>(std::forward<Rest>(rest)...);
		modifiers.insert(modifiers.end(), std::make_move_iterator(rest.begin()), std::make_move_iterator(rest.end()));
		return modifiers;
	}

	//! @todo Why do the following lambda overload helpers not work?

	/*template <typename... Functors>
	struct overloaded : Functors...
	{
		using Functors::operator ()...;
	};

	template <typename... Functors>
	overloaded<Functors...> overload(Functors...)
	{
		return overloaded<Functors...>{};
	}*/
}
