//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Miscellaneous utility functions.

#pragma once

#include "reference.hpp"

#include "ui/view_space.hpp"

namespace ql {
	//! Useful for selectively disabling constexpr branches.
	template <typename T>
	struct dependent_false_type : std::false_type {};

	//! Scales a value in [0, 1] to [0, 255].
	template <typename T>
	auto to_uint8(T percentage) -> sf::Uint8 {
		return static_cast<sf::Uint8>(255 * percentage);
	}

	//! Moves @p elements into a new vector and returns it.
	//! @note The vector's element type is determined by the first element. @p elements must by nonempty.
	template <typename... Ts>
	auto make_vector(Ts... elements) {
		std::vector<std::tuple_element_t<0, std::tuple<Ts...>>> result;
		(result.push_back(std::move(elements)), ...);
		return result;
	}
}
