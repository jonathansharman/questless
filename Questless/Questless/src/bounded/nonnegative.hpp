//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

#include <algorithm>

namespace ql {
	//! Wraps an arithmetic type, clamping negative values to zero.
	template <typename ArithmeticType>
	struct nonnegative : bounded<ArithmeticType, nonnegative<ArithmeticType>> {
		using arithmetic_type = ArithmeticType;

		constexpr nonnegative() noexcept = default;

		constexpr nonnegative(nonnegative const&) noexcept = default;
		constexpr nonnegative(nonnegative&&) noexcept = default;

		constexpr nonnegative(arithmetic_type const& value) noexcept : _value{std::max(arithmetic_type(0), value)} {}

		constexpr auto& operator=(nonnegative const& bounded) noexcept {
			set_value(bounded.value());
			return *this;
		}
		constexpr auto& operator=(nonnegative&& bounded) noexcept {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr auto& operator=(arithmetic_type const& value) noexcept {
			set_value(value);
			return *this;
		}
		constexpr auto& operator=(arithmetic_type&& value) noexcept {
			set_value(std::move(value));
			return *this;
		}

		//! The contained value.
		constexpr auto value() const noexcept {
			return _value;
		}

		//! Sets the contained value to max(0, @p value).
		constexpr auto set_value(arithmetic_type const& value) noexcept -> void {
			_value = std::max(arithmetic_type(0), value);
		}

	private:
		arithmetic_type _value{};
	};
}
