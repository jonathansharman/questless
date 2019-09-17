//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>

namespace ql {
	//! An inclusive range of arithmetic values, clamped between zero and a dynamic upper bound.
	//! @tparam ArithmeticType The underlying type.
	//! @note See also @p dynamic_bounded and @p nonnegative.
	template <typename ArithmeticType>
	struct dynamic_nonnegative : bounded<ArithmeticType, dynamic_nonnegative<ArithmeticType>> {
		using arithmetic_type = ArithmeticType;

		//! Constructs with value and upper bound initialized to zero.
		constexpr dynamic_nonnegative() noexcept : _upper_bound{arithmetic_type(0)}, _value{arithmetic_type(0)} {}

		//! @param value The initial value, which will be clamped to within the range.
		//! @param upper_bound The maximum value in the range (inclusive).
		//! @note If @p upper_bound is negative, the upper bound will be set to zero.
		constexpr dynamic_nonnegative(arithmetic_type const& value, arithmetic_type const& upper_bound) noexcept
			: _upper_bound{std::max(arithmetic_type(0), upper_bound)}
			, _value{std::clamp(value, arithmetic_type(0), _upper_bound)} {}

		constexpr dynamic_nonnegative(dynamic_nonnegative const&) noexcept = default;
		constexpr dynamic_nonnegative(dynamic_nonnegative&& other) noexcept = default;

		//! Assigns the value from @p bounded to this value. Does not affect the upper bound.
		constexpr dynamic_nonnegative& operator=(dynamic_nonnegative const& bounded) noexcept {
			set_value(bounded.value());
			return *this;
		}
		//! Assigns the value from @p bounded to this value. Does not affect the upper bound.
		constexpr dynamic_nonnegative& operator=(dynamic_nonnegative&& bounded) noexcept {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr dynamic_nonnegative& operator=(arithmetic_type const& value) noexcept {
			set_value(value);
			return *this;
		}
		constexpr dynamic_nonnegative& operator=(arithmetic_type&& value) noexcept {
			set_value(std::move(value));
			return *this;
		}

		//! Implicit conversion to the contained type.
		constexpr operator arithmetic_type() const noexcept {
			return _value;
		}

		//! The contained value.
		constexpr auto value() const noexcept {
			return _value;
		}

		//! The maximum value in the range (inclusive).
		constexpr auto const& upper_bound() const noexcept {
			return _upper_bound;
		}

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr auto set_value(arithmetic_type const& value) noexcept -> void {
			_value = std::clamp(value, arithmetic_type(0), _upper_bound);
		}

		//! Sets the inclusive maximum value to the maximum of @p upper_bound and zero and clamps the contained value if
		//! necessary.
		constexpr auto set_upper_bound(arithmetic_type const& upper_bound) noexcept -> void {
			_upper_bound = std::max(arithmetic_type(0), upper_bound);
			if (_value > _upper_bound) { _value = _upper_bound; }
		}

	private:
		arithmetic_type _upper_bound;
		arithmetic_type _value;
	};
}
