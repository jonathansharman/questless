//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>

namespace ql {
	//! An inclusive, dynamically bounded range of arithmetic values, clamping values outside the range to the nearest
	//! valid value.
	//! @tparam ArithmeticType The underlying type.
	//! @note See also @p static_bounded and @p lazy_bounded.
	template <typename ArithmeticType>
	struct dynamic_bounded : bounded<ArithmeticType, dynamic_bounded<ArithmeticType>> {
		using arithmetic_type = ArithmeticType;

		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		//! @note If @p upper_bound < @p lower_bound, the upper bound will be set to @p lower_bound.
		constexpr dynamic_bounded(arithmetic_type lower_bound, arithmetic_type const& upper_bound) noexcept
			: _lower_bound{std::move(lower_bound)}, _upper_bound{std::max(_lower_bound, upper_bound)} {}

		//! @param value The initial value, which will be clamped to within the range.
		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		//! @note If @p upper_bound < @p lower_bound, the upper bound will be set to @p lower_bound.
		constexpr dynamic_bounded(arithmetic_type const& value, arithmetic_type lower_bound, arithmetic_type const& upper_bound) noexcept
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::max(_lower_bound, upper_bound)}
			, _value{std::clamp(value, lower_bound, upper_bound)} {}

		constexpr dynamic_bounded(dynamic_bounded const&) noexcept = default;
		constexpr dynamic_bounded(dynamic_bounded&& other) noexcept = default;

		//! Assigns the value from @p bounded to this value. Does not affect the lower and upper bounds.
		constexpr auto& operator=(dynamic_bounded const& bounded) noexcept {
			set(bounded.get());
			return *this;
		}
		//! Assigns the value from @p bounded to this value. Does not affect the lower and upper bounds.
		constexpr auto& operator=(dynamic_bounded&& bounded) noexcept {
			set(std::move(bounded.get()));
			return *this;
		}
		constexpr auto& operator=(arithmetic_type const& value) noexcept {
			set(value);
			return *this;
		}
		constexpr dynamic_bounded& operator=(arithmetic_type&& value) noexcept {
			set(std::move(value));
			return *this;
		}

		//! Implicit conversion to the contained type.
		constexpr operator arithmetic_type() const noexcept {
			return _value;
		}

		//! The contained value.
		constexpr auto get() const noexcept {
			return _value;
		}

		//! The minimum value in the range (inclusive).
		constexpr auto const& lower_bound() const noexcept {
			return _lower_bound;
		}

		//! The maximum value in the range (inclusive).
		constexpr auto const& upper_bound() const noexcept {
			return _upper_bound;
		}

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr auto set(arithmetic_type const& value) -> void noexcept {
			_value = std::clamp(value, _lower_bound, _upper_bound);
		}

		//! Sets the inclusive minimum value to the minimum of @p lower_bound and the current upper bound and clamps the
		//! contained value if necessary.
		constexpr auto set_lower_bound(arithmetic_type const& lower_bound) -> void noexcept {
			_lower_bound = std::min(lower_bound, _upper_bound);
			if (_value < _lower_bound) { _value = _lower_bound; }
		}

		//! Sets the inclusive maximum value to the maximum of @p upper_bound and the current lower bound and clamps the
		//! contained value if necessary.
		constexpr auto set_upper_bound(arithmetic_type const& upper_bound) -> void noexcept {
			_upper_bound = std::max(_lower_bound, upper_bound);
			if (_value > _upper_bound) { _value = _upper_bound; }
		}

	private:
		arithmetic_type _lower_bound;
		arithmetic_type _upper_bound;
		arithmetic_type _value;
	};
}
