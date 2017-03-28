/**
* @file    DynamicProperty.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION
*
* A property type, inspired by C#-style properties. Provides accessors and mutators to an underlying value. All mutators call the provided
* mutator() function to ensure this value remains in a valid state. The mutator may be stateful and may be altered at run time using
* set_mutator(). If your mutator is known at compile time and can be expressed as a stateless function, consider using Property instead
* of DynamicProperty for better performance.
*
* The mutator takes the current value by reference and the new value by const-reference and is responsible for assigning the new value to
* the current value, handling any domain errors appropriately.
*/

#pragma once

#include <functional>
#include <istream>

namespace questless
{
	template <typename ValueType>
	class DynamicProperty
	{
	public:
		using value_t = ValueType;
		using mutator_t = std::function<void(value_t&, value_t const&)>;

		// Constructors

		/// Constructs a property using the default value of its value type and an empty mutator.
		/// @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr DynamicProperty() = default;

		constexpr DynamicProperty(DynamicProperty const& property) = default;
		constexpr DynamicProperty(DynamicProperty&& property) = default;

		/// Constructs a property using the given value and an empty mutator.
		/// @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr DynamicProperty(value_t value)
			: _value{std::move(value)}
			, _mutator{}
		{}
		/// Constructs a property using the mutated default value of its value type.
		constexpr DynamicProperty(mutator_t mutator)
			: _value{}
			, _mutator{std::move(mutator)}
		{
			_mutator(_value, _value);
		}
		/// Constructs a property from the given value and mutator.
		constexpr DynamicProperty(value_t value, mutator_t mutator)
			: _value{std::move(value)}
			, _mutator{std::move(mutator)}
		{
			_mutator(_value, _value);
		}

		/// Sets the property's mutator function to the given function.
		/// @param mutator The new function to use as the mutator.
		/// @param mutate_now Whether to call the new mutator on the current value.
		void set_mutator(mutator_t mutator, bool mutate_now = true)
		{
			_mutator = std::move(mutator);
			if (mutate_now) {
				_mutator(_value, _value);
			}
		}

		// Accessors and Mutators

		/// Sets the property's underlying value to the given value and calls the mutator on it.
		/// @param value The property's new value.
		void set(value_t const& value) { _mutator(_value, value); }

		/// @return The property's underlying value.
		constexpr operator value_t const&() const { return _value; }

		/// @return The property's underlying value.
		constexpr value_t const& get() const { return _value; }

		/// @return The property's underlying value, dereferenced. Requires an underlying type that supports dereference.
		constexpr decltype(auto) operator *() const { return *_value; }

		/// @return The address of the property's underlying value.
		constexpr const value_t* operator ->() const { return &_value; }

		// Asignment

		DynamicProperty& operator =(DynamicProperty const& property)
		{
			set(property._value);
			return *this;
		}
		DynamicProperty& operator =(value_t value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		DynamicProperty& operator +=(T const& that)
		{
			set(_value + that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator -=(T const& that)
		{
			set(_value - that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator *=(T const& that)
		{
			set(_value * that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator /=(T const& that)
		{
			set(_value / that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator %=(T const& that)
		{
			set(_value % that);
			return *this;
		}

		// Bitwise Assignment Operators

		template <typename T>
		DynamicProperty& operator &=(T const& that)
		{
			set(_value & that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator |=(T const& that)
		{
			set(_value | that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator ^=(T const& that)
		{
			set(_value | that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator <<=(T const& that)
		{
			set(_value << that);
			return *this;
		}
		template <typename T>
		DynamicProperty& operator >>=(T const& that)
		{
			set(_value >> that);
			return *this;
		}

		// Increment/decrement Operators

		DynamicProperty& operator ++()
		{
			set(_value + 1);
			return *this;
		}
		DynamicProperty& operator --()
		{
			set(_value - 1);
			return *this;
		}
		DynamicProperty operator ++(int)
		{
			value_t value = _value;
			set(_value + 1);
			return value;
		}
		DynamicProperty operator --(int)
		{
			value_t value = _value;
			set(_value - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, DynamicProperty& property)
		{
			value_t value;
			in >> value;
			property.set(value);
			return in;
		}
	private:
		value_t _value;
		mutator_t _mutator;
	};
}
