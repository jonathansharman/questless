//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>
#include <limits>

namespace ql {
	namespace detail {
		template <typename ArithmeticType>
		struct maximum {};

		template <>
		struct maximum<int> {
			static constexpr int value = ::std::numeric_limits<int>::max();
		};
		template <>
		struct maximum<double> {
			static constexpr double value = ::std::numeric_limits<double>::max();
		};
	}

	//! An inclusive, statically bounded range of arithmetic values, clamping values outside the range to the nearest
	//! valid value.
	//! @tparam ArithmeticType The underlying type.
	//! @tparam LowerBound The minimum value in the range (inclusive).
	//! @tparam UpperBound The maximum value in the range (inclusive).
	//! @note See also @p dynamic_bounded and @p lazy_bounded.
	template <typename ArithmeticType, ArithmeticType const& LowerBound, ArithmeticType const& UpperBound = detail::maximum<ArithmeticType>::value>
	struct static_bounded : bounded<ArithmeticType, static_bounded<ArithmeticType, LowerBound, UpperBound>> {
		using arithmetic_type = ArithmeticType;

		//! The minimum value in the range (inclusive).
		static constexpr arithmetic_type lower_bound = LowerBound;

		//! The maximum value in the range (inclusive).
		static constexpr arithmetic_type upper_bound = UpperBound;

		constexpr static_bounded() noexcept = default;

		constexpr static_bounded(static_bounded const&) noexcept = default;
		constexpr static_bounded(static_bounded&&) noexcept = default;

		constexpr static_bounded(arithmetic_type const& value) noexcept
			: _value{std::clamp(value, lower_bound, upper_bound)} {}

		constexpr static_bounded& operator=(static_bounded const& bounded) noexcept {
			set_value(bounded.value());
			return *this;
		}
		constexpr static_bounded& operator=(static_bounded&& bounded) noexcept {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr static_bounded& operator=(arithmetic_type const& value) noexcept {
			set_value(value);
			return *this;
		}
		constexpr static_bounded& operator=(arithmetic_type&& value) noexcept {
			set_value(std::move(value));
			return *this;
		}

		//! The contained value.
		constexpr arithmetic_type value() const noexcept {
			return _value;
		}

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr void set_value(arithmetic_type const& value) noexcept {
			constexpr auto min = std::numeric_limits<arithmetic_type>::min();
			constexpr auto max = std::numeric_limits<arithmetic_type>::max();
			if constexpr (lower_bound == min && upper_bound == max) {
				// No checks.
				_value = value;
			} else if constexpr (lower_bound != min && upper_bound == max) {
				// Only check lower bound.
				_value = std::max(value, lower_bound);
			} else if constexpr (lower_bound == min && upper_bound != max) {
				// Only check upper bound.
				_value = std::min(value, upper_bound);
			} else {
				// Check both bounds.
				_value = std::clamp(value, lower_bound, upper_bound);
			}
		}

	private:
		arithmetic_type _value{};
	};
}
