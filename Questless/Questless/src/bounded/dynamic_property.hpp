//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

namespace ql {
	//! @brief A property type, inspired by C#-style properties. Supports dynamic mutators.
	//! @tparam ValueType The underlying value type.
	//!
	//! Provides accessors and mutators to an underlying value. All mutators call the provided @p mutator() function to
	//! ensure this value remains in a valid state. The mutator may be stateful and may be altered at run time using @p
	//! set_mutator(). If your mutator is known at compile time and can be expressed as a stateless function, consider
	//! using @p static_property instead of @p dynamic_property for better performance.
	//!
	//! The mutator takes the current value by reference and the new value by const-reference and is responsible for
	//! assigning the new value to the current value, handling any domain errors appropriately.
	template <typename ValueType>
	struct dynamic_property : bounded<ValueType, dynamic_property<ValueType>> {
		using value_type = ValueType;
		using mutator_type = std::function<void(value_type&, value_type const&)>;

		//! Constructs a property using the default value of its value type and an empty mutator.
		//! @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr dynamic_property() noexcept = default;

		constexpr dynamic_property(dynamic_property const& property) noexcept = default;
		constexpr dynamic_property(dynamic_property&& property) noexcept = default;

		//! Constructs a property using the given value and an empty mutator.
		//! @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr dynamic_property(value_type value) noexcept : _mutator{}, _value{std::move(value)} {}
		//! Constructs a property using the mutated default value of its value type.
		constexpr dynamic_property(mutator_type mutator) noexcept : _mutator{std::move(mutator)}, _value{} {
			_mutator(_value, _value);
		}
		//! Constructs a property from the given value and mutator.
		constexpr dynamic_property(value_type value, mutator_type mutator) noexcept
			: _mutator{std::move(mutator)}, _value{std::move(value)} {
			_mutator(_value, _value);
		}

		constexpr dynamic_property& operator=(dynamic_property const& bounded) noexcept {
			set_value(bounded.value());
			return *this;
		}
		constexpr dynamic_property& operator=(dynamic_property&& bounded) noexcept {
			set_value(std::move(bounded.value()));
			return *this;
		}
		constexpr dynamic_property& operator=(value_type const& value) noexcept {
			set_value(value);
			return *this;
		}
		constexpr dynamic_property& operator=(value_type&& value) noexcept {
			set_value(std::move(value));
			return *this;
		}

		//! The property's underlying value.
		constexpr value_type const& value() const noexcept {
			return _value;
		}

		//! Sets the property's underlying value to the given value and calls the mutator on it.
		//! @param value The property's new value.
		void set_value(value_type const& value) noexcept {
			_mutator(_value, value);
		}

		//! Sets the property's mutator function to the given function.
		//! @param mutator The new function to use as the mutator.
		//! @param mutate_now Whether to call the new mutator on the current value.
		void set_mutator(mutator_type mutator, bool mutate_now = true) noexcept {
			_mutator = std::move(mutator);
			if (mutate_now) { _mutator(_value, _value); }
		}

	private:
		mutator_type _mutator;
		value_type _value{};
	};
}
