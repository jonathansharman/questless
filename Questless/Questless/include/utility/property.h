//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <istream>

namespace ql
{
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
	class property
	{
	public:
		using value_type = ValueType;

		//////////////////
		// Constructors //
		//////////////////

		constexpr property() = default;

		constexpr property(property const& property) = default;
		constexpr property(property&& property) = default;

		constexpr property(value_type value) : _value{std::move(value)} { mutator(_value, _value); }

		////////////////////////////
		// Accessors and Mutators //
		////////////////////////////

		//! The property's underlying value.
		constexpr operator value_type const&() const { return _value; }

		//! The property's underlying value.
		constexpr value_type const& value() const { return _value; }

		//! The property's underlying value, dereferenced. Requires an underlying type that supports dereference.
		constexpr decltype(auto) operator *() const { return *_value; }

		//! The address of the property's underlying value.
		constexpr const value_type* operator ->() const { return &_value; }

		//! Sets the property's underlying value to the given value and calls the mutator on it.
		//! @param value The property's new value.
		void set_value(value_type const& value) { mutator(_value, value); }

		///////////////
		// Asignment //
		///////////////

		property& operator =(property const& property)
		{
			set_value(property._value);
			return *this;
		}
		property& operator =(value_type value)
		{
			set_value(value);
			return *this;
		}

		/////////////////////////////////////
		// Arithmetic Assignment Operators //
		/////////////////////////////////////

		template <typename T>
		property& operator +=(T const& that)
		{
			set_value(_value + that);
			return *this;
		}
		template <typename T>
		property& operator -=(T const& that)
		{
			set_value(_value - that);
			return *this;
		}
		template <typename T>
		property& operator *=(T const& that)
		{
			set_value(_value * that);
			return *this;
		}
		template <typename T>
		property& operator /=(T const& that)
		{
			set_value(_value / that);
			return *this;
		}
		template <typename T>
		property& operator %=(T const& that)
		{
			set_value(_value % that);
			return *this;
		}

		//////////////////////////////////
		// Bitwise Assignment Operators //
		//////////////////////////////////

		template <typename T>
		property& operator &=(T const& that)
		{
			set_value(_value & that);
			return *this;
		}
		template <typename T>
		property& operator |=(T const& that)
		{
			set_value(_value | that);
			return *this;
		}
		template <typename T>
		property& operator ^=(T const& that)
		{
			set_value(_value | that);
			return *this;
		}
		template <typename T>
		property& operator <<=(T const& that)
		{
			set_value(_value << that);
			return *this;
		}
		template <typename T>
		property& operator >>=(T const& that)
		{
			set_value(_value >> that);
			return *this;
		}

		///////////////////////////////////
		// Increment/decrement Operators //
		///////////////////////////////////

		property& operator ++()
		{
			set_value(_value + 1);
			return *this;
		}
		property& operator --()
		{
			set_value(_value - 1);
			return *this;
		}
		property operator ++(int)
		{
			value_type value = _value;
			set_value(_value + 1);
			return value;
		}
		property operator --(int)
		{
			value_type value = _value;
			set_value(_value - 1);
			return value;
		}

		////////////////////////////////
		// Stream Extraction Operator //
		////////////////////////////////

		friend std::istream& operator >>(std::istream& in, property& property)
		{
			value_type value;
			in >> value;
			property.set_value(value);
			return in;
		}
	private:
		value_type _value;
	};
}
