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
	sf::Uint8 to_uint8(T percentage) {
		return static_cast<sf::Uint8>(255 * percentage);
	}

	//! Moves @p pointer into a new vector and returns it.
	template <typename T, typename U>
	auto make_uptr_vector(uptr<U> pointer) {
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(pointer));
		return modifiers;
	}

	//! Moves the given pointers into a new vector and returns it.
	template <typename T, typename First, typename... Rest>
	auto make_uptr_vector(First first, Rest... rest) {
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(first));
		auto restVector = make_uptr_vector<T>(std::forward<Rest>(rest)...);
		modifiers.insert( //
			modifiers.end(),
			std::make_move_iterator(restVector.begin()),
			std::make_move_iterator(restVector.end()));
		return modifiers;
	}

	//! Generates a new vector by applying @p f to each element of @p source_vector.
	//! @param source_vector The vector from which to generate the transformed vector.
	//! @param f A function from the source vector element type to the result vector element type.
	//! @return The transformed vector.
	template <typename T, typename UnaryFunction>
	auto transformed_vector(std::vector<T> source_vector, UnaryFunction&& f) {
		std::vector<std::invoke_result_t<UnaryFunction, T>> result;
		for (T const& element : source_vector) {
			result.push_back(f(element));
		};
		return result;
	}

	//! Computes the ratio of quantities @p q1 and @p q2 after casting their representations to @p RatioType.
	//! Useful when the ratio between two quantities needs to use a wider type than @p Rep.
	template <typename RatioType, typename Rep, typename Unit>
	auto ratio(cancel::quantity<Rep, Unit> const& q1, cancel::quantity<Rep, Unit> const& q2) {
		return static_cast<RatioType>(q1.value) / static_cast<RatioType>(q2.value);
	}

	//! Scales @p q by @p scale via static casting. Useful for scaling operations that require narrowing conversions.
	template <typename ScaleType, typename Rep, typename Unit>
	auto scale(cancel::quantity<Rep, Unit> const& q, ScaleType const& scale) {
		using common = std::common_type_t<ScaleType, Rep>;
		return cancel::quantity<Rep, Unit>(static_cast<Rep>(static_cast<common>(q.value) * static_cast<common>(scale)));
	}

	//! Scales @p q by @p scale via static casting. Useful for scaling operations that require narrowing conversions.
	template <typename ScaleType, typename Rep, typename Unit>
	auto scale(ScaleType const& scale, cancel::quantity<Rep, Unit> const& q) {
		using common = std::common_type_t<ScaleType, Rep>;
		return cancel::quantity<Rep, Unit>(static_cast<Rep>(static_cast<common>(scale) * static_cast<common>(q.value)));
	}
}
