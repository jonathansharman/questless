//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded.hpp"

namespace ql {
	//! @brief A property type, inspired by C#-style properties. Supports only static mutators.
	//! @tparam ValueType The underlying value type.
	//! @tparam mutator The mutator function to call when the property is changed.
	//!
	//! Provides accessors and mutators to an underlying value. All mutators call the provided @p mutator() function to ensure this value remains
	//! in a valid state. The mutator must be a function pointer. If a dynamic mutator is required, e.g., if you want to use a lambda with a
	//! capture or change the mutator during execution, use dynamic_property instead.
	//!
	//! The mutator takes the current value by reference and the new value by const-reference and is responsible for assigning the new value to
	//! the current value, handling any domain errors appropriately.
	template <typename ValueType, void (*mutator)(ValueType&, ValueType const&)>
	class static_property : public bounded<ValueType, static_property<ValueType, mutator>> {
	public:
		using value_type = ValueType;

		constexpr static_property() = default;

		constexpr static_property(static_property const&) = default;
		constexpr static_property(static_property&&) = default;

		constexpr static_property(value_type value) : _value{std::move(value)} { mutator(_value, _value); }

		constexpr static_property& operator =(static_property const& bounded) {
			set_value(bounded.value());
			return *this;
		}
		constexpr static_property& operator =(static_property&& bounded) {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr static_property& operator =(value_type const& value) {
			set_value(value);
			return *this;
		}
		constexpr static_property& operator =(value_type&& value) {
			set_value(std::move(value));
			return *this;
		}

		//! The property's underlying value.
		constexpr value_type const& value() const { return _value; }

		//! Sets the property's underlying value to @p value and calls the mutator on it.
		constexpr void set_value(value_type const& value) { mutator(_value, value); }
	private:
		value_type _value;
	};
}
