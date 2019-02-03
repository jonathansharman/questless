//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>

namespace ql {
	//! An inclusive, dynamically bounded range of arithmetic values, clamping values outside the range to the nearest valid value.
	//! @tparam ArithmeticType The underlying type.
	//! @note See also @p static_bounded and @p lazy_bounded.
	template <typename ArithmeticType>
	struct dynamic_bounded : bounded<ArithmeticType, dynamic_bounded<ArithmeticType>> {
		using arithmetic_type = ArithmeticType;

		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		constexpr dynamic_bounded(arithmetic_type lower_bound, arithmetic_type upper_bound)
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::move(upper_bound)}
		{}

		//! @param value The initial value, which will be clamped to within the range.
		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		constexpr dynamic_bounded(arithmetic_type value, arithmetic_type lower_bound, arithmetic_type upper_bound)
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::move(upper_bound)}
			, _value{std::clamp(value, lower_bound, upper_bound)}
		{}

		constexpr dynamic_bounded(dynamic_bounded const&) = default;
		constexpr dynamic_bounded(dynamic_bounded&& other) = default;

		constexpr dynamic_bounded& operator =(dynamic_bounded const& bounded) {
			set_value(bounded.value());
			return *this;
		}
		constexpr dynamic_bounded& operator =(dynamic_bounded&& bounded) {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr dynamic_bounded& operator =(arithmetic_type const& value) {
			set_value(value);
			return *this;
		}
		constexpr dynamic_bounded& operator =(arithmetic_type&& value) {
			set_value(std::move(value));
			return *this;
		}

		//! The contained value.
		constexpr arithmetic_type value() const { return _value; }

		//! The minimum value in the range (inclusive).
		constexpr arithmetic_type const& lower_bound() const { return _lower_bound; }

		//! The maximum value in the range (inclusive).
		constexpr arithmetic_type const& upper_bound() const { return _upper_bound; }

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr void set_value(arithmetic_type const& value) {
			_value = std::clamp(value, _lower_bound, _upper_bound);
		}

		//! Sets the inclusive minimum value in the range to @p lower_bound and clamps the contained value if necessary.
		constexpr void set_lower_bound(arithmetic_type lower_bound) {
			_lower_bound = std::move(lower_bound);
			if (_value < _lower_bound) {
				_value = _lower_bound;
			}
		}

		//! Sets the inclusive maximum value in the range to @p upper_bound and clamps the contained value if necessary.
		constexpr void set_upper_bound(arithmetic_type upper_bound) {
			_upper_bound = std::move(upper_bound);
			if (_value > _upper_bound) {
				_value = _upper_bound;
			}
		}
	private:
		arithmetic_type _lower_bound;
		arithmetic_type _upper_bound;
		arithmetic_type _value;
	};
}
