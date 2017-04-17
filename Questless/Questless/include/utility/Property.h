//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief A property type, inspired by C#-style properties. Supports only static mutators.
//!
//! Provides accessors and mutators to an underlying value. All mutators call the provided mutator() function to ensure this value remains
//! in a valid state. The mutator must be a function pointer. If a dynamic mutator is required, e.g., if you want to use a lambda with a
//! capture or change the mutator during execution, use DynamicProperty instead.
//!
//! The mutator takes the current value by reference and the new value by const-reference and is responsible for assigning the new value to
//! the current value, handling any domain errors appropriately.

#pragma once

#include <istream>

namespace questless
{
	template <typename ValueType, void (*mutator)(ValueType&, ValueType const&)>
	class Property
	{
	public:
		using value_t = ValueType;

		// Constructors

		constexpr Property() = default;

		constexpr Property(Property const& property) = default;
		constexpr Property(Property&& property) = default;

		constexpr Property(value_t value) : _value{std::move(value)} { mutator(_value, _value); }

		// Accessors and Mutators

		//! Sets the property's underlying value to the given value and calls the mutator on it.
		//! @param value The property's new value.
		void set(value_t const& value) { mutator(_value, value); }

		//! The property's underlying value.
		constexpr operator value_t const&() const { return _value; }

		//! The property's underlying value.
		constexpr value_t const& get() const { return _value; }

		//! The property's underlying value, dereferenced. Requires an underlying type that supports dereference.
		constexpr decltype(auto) operator *() const { return *_value; }

		//! The address of the property's underlying value.
		constexpr const value_t* operator ->() const { return &_value; }

		// Asignment

		Property& operator =(Property const& property)
		{
			set(property._value);
			return *this;
		}
		Property& operator =(value_t value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		Property& operator +=(T const& that)
		{
			set(_value + that);
			return *this;
		}
		template <typename T>
		Property& operator -=(T const& that)
		{
			set(_value - that);
			return *this;
		}
		template <typename T>
		Property& operator *=(T const& that)
		{
			set(_value * that);
			return *this;
		}
		template <typename T>
		Property& operator /=(T const& that)
		{
			set(_value / that);
			return *this;
		}
		template <typename T>
		Property& operator %=(T const& that)
		{
			set(_value % that);
			return *this;
		}

		// Bitwise Assignment Operators

		template <typename T>
		Property& operator &=(T const& that)
		{
			set(_value & that);
			return *this;
		}
		template <typename T>
		Property& operator |=(T const& that)
		{
			set(_value | that);
			return *this;
		}
		template <typename T>
		Property& operator ^=(T const& that)
		{
			set(_value | that);
			return *this;
		}
		template <typename T>
		Property& operator <<=(T const& that)
		{
			set(_value << that);
			return *this;
		}
		template <typename T>
		Property& operator >>=(T const& that)
		{
			set(_value >> that);
			return *this;
		}

		// Increment/decrement Operators

		Property& operator ++()
		{
			set(_value + 1);
			return *this;
		}
		Property& operator --()
		{
			set(_value - 1);
			return *this;
		}
		Property operator ++(int)
		{
			value_t value = _value;
			set(_value + 1);
			return value;
		}
		Property operator --(int)
		{
			value_t value = _value;
			set(_value - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, Property& property)
		{
			value_t value;
			in >> value;
			property.set(value);
			return in;
		}
	private:
		value_t _value;
	};
}
