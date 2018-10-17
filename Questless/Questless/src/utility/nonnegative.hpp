//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>

namespace ql {
	//! Wraps an arithmetic type, clamping negative values to zero.
	template <typename ArithmeticType>
	class nonnegative : public bounded<ArithmeticType, nonnegative<ArithmeticType>> {
	public:
		using arithmetic_type = ArithmeticType;

		constexpr nonnegative() = default;

		constexpr nonnegative(nonnegative const&) = default;
		constexpr nonnegative(nonnegative&&) = default;

		constexpr nonnegative(arithmetic_type const& value) : _value{std::max(arithmetic_type(0), value)} {}

		constexpr nonnegative& operator =(nonnegative const& bounded) {
			set_value(bounded.value());
			return *this;
		}
		constexpr nonnegative& operator =(nonnegative&& bounded) noexcept {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr nonnegative& operator =(arithmetic_type const& value) {
			set_value(value);
			return *this;
		}
		constexpr nonnegative& operator =(arithmetic_type&& value) {
			set_value(std::move(value));
			return *this;
		}

		//! The contained value.
		constexpr arithmetic_type value() const { return _value; }

		//! Sets the contained value to max(0, @p value).
		constexpr void set_value(arithmetic_type const& value) { _value = std::max(arithmetic_type(0), value); }
	private:
		arithmetic_type _value;
	};
}
