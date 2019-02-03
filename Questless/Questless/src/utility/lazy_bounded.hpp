//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <functional>
#include <istream>

namespace ql {
	//! An inclusive range of arithmetic values bounded lazily to limits defined by function results.
	//! Values outside the range are clamped to the nearest valid value on read.
	//! @tparam ArithmeticType The underlying type.
	//! @note See also @p static_bounded and @p dynamic_bounded.
	template <typename ArithmeticType>
	struct lazy_bounded : bounded<ArithmeticType, lazy_bounded<ArithmeticType>> {
		using arithmetic_type = ArithmeticType;

		using bound_getter_type = std::function<arithmetic_type()>;

		//! The function for getting the inclusive minimum value in the range.
		std::function<arithmetic_type()> lower_bound_getter;
		//! The function for getting the inclusive maximum value in the range.
		std::function<arithmetic_type()> upper_bound_getter;

		constexpr lazy_bounded(bound_getter_type lower_bound_getter, bound_getter_type upper_bound_getter) noexcept
			: lower_bound_getter{std::move(lower_bound_getter)}
			, upper_bound_getter{std::move(upper_bound_getter)}
		{}

		constexpr lazy_bounded(arithmetic_type value, bound_getter_type lower_bound_getter, bound_getter_type upper_bound_getter)
			: lower_bound_getter{std::move(lower_bound_getter)}
			, upper_bound_getter{std::move(upper_bound_getter)}
			, _value{std::move(value)}
		{}

		constexpr lazy_bounded(lazy_bounded const&) = default;

		constexpr lazy_bounded(lazy_bounded&& other) noexcept
			: lower_bound_getter{std::move(other.lower_bound_getter)}
			, upper_bound_getter{std::move(other.upper_bound_getter)}
			, _value{std::move(other._value)}
		{}

		constexpr lazy_bounded& operator =(lazy_bounded const& bounded) {
			set_value(bounded.value());
			return *this;
		}
		constexpr lazy_bounded& operator =(lazy_bounded&& bounded) {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr lazy_bounded& operator =(arithmetic_type const& value) {
			set_value(value);
			return *this;
		}
		constexpr lazy_bounded& operator =(arithmetic_type&& value) {
			set_value(std::move(value));
			return *this;
		}

		//! The contained value.
		constexpr arithmetic_type value() const {
			auto result = std::clamp(_value, lower_bound_getter(), upper_bound_getter());
			return result;
		}

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr void set_value(arithmetic_type const& value) { _value = value; }

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr void set_value(arithmetic_type&& value) { _value = std::move(value); }
	private:
		arithmetic_type _value;
	};
}

//! @todo Tests.
