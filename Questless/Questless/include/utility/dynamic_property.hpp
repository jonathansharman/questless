//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <istream>

namespace ql
{
	//! @brief A property type, inspired by C#-style properties. Supports dynamic mutators.
	//! @tparam ValueType The underlying value type.
	//!
	//! Provides accessors and mutators to an underlying value. All mutators call the provided @p mutator() function to ensure this value remains
	//! in a valid state. The mutator may be stateful and may be altered at run time using @p set_mutator(). If your mutator is known at compile
	//! time and can be expressed as a stateless function, consider using @p property instead of @p dynamic_property for better performance.
	//!
	//! The mutator takes the current value by reference and the new value by const-reference and is responsible for assigning the new value to
	//! the current value, handling any domain errors appropriately.
	template <typename ValueType>
	class dynamic_property
	{
	public:
		using value_type = ValueType;
		using mutator_type = std::function<void(value_type&, value_type const&)>;

		//////////////////
		// Constructors //
		//////////////////

		//! Constructs a property using the default value of its value type and an empty mutator.
		//! @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr dynamic_property() = default;

		constexpr dynamic_property(dynamic_property const& property) = default;
		constexpr dynamic_property(dynamic_property&& property) = default;

		//! Constructs a property using the given value and an empty mutator.
		//! @note Calling a property mutator without first setting the mutator results in a run-time error.
		constexpr dynamic_property(value_type value)
			: _mutator{}
			, _value{std::move(value)}
		{}
		//! Constructs a property using the mutated default value of its value type.
		constexpr dynamic_property(mutator_type mutator)
			: _mutator{std::move(mutator)}
			, _value{}
		{
			_mutator(_value, _value);
		}
		//! Constructs a property from the given value and mutator.
		constexpr dynamic_property(value_type value, mutator_type mutator)
			: _mutator{std::move(mutator)}
			, _value{std::move(value)}
		{
			_mutator(_value, _value);
		}

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
		void set_value(value_type const& value) { _mutator(_value, value); }

		//! Sets the property's mutator function to the given function.
		//! @param mutator The new function to use as the mutator.
		//! @param mutate_now Whether to call the new mutator on the current value.
		void set_mutator(mutator_type mutator, bool mutate_now = true)
		{
			_mutator = std::move(mutator);
			if (mutate_now) {
				_mutator(_value, _value);
			}
		}

		///////////////
		// Asignment //
		///////////////

		dynamic_property& operator =(dynamic_property const& property)
		{
			set_value(property._value);
			return *this;
		}
		dynamic_property& operator =(value_type value)
		{
			set_value(value);
			return *this;
		}

		/////////////////////////////////////
		// Arithmetic Assignment Operators //
		/////////////////////////////////////

		template <typename T>
		dynamic_property& operator +=(T const& that)
		{
			set_value(_value + that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator -=(T const& that)
		{
			set_value(_value - that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator *=(T const& that)
		{
			set_value(_value * that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator /=(T const& that)
		{
			set_value(_value / that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator %=(T const& that)
		{
			set_value(_value % that);
			return *this;
		}

		//////////////////////////////////
		// Bitwise Assignment Operators //
		//////////////////////////////////

		template <typename T>
		dynamic_property& operator &=(T const& that)
		{
			set_value(_value & that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator |=(T const& that)
		{
			set_value(_value | that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator ^=(T const& that)
		{
			set_value(_value | that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator <<=(T const& that)
		{
			set_value(_value << that);
			return *this;
		}
		template <typename T>
		dynamic_property& operator >>=(T const& that)
		{
			set_value(_value >> that);
			return *this;
		}

		///////////////////////////////////
		// Increment/decrement Operators //
		///////////////////////////////////

		dynamic_property& operator ++()
		{
			set_value(_value + 1);
			return *this;
		}
		dynamic_property& operator --()
		{
			set_value(_value - 1);
			return *this;
		}
		dynamic_property operator ++(int)
		{
			value_type value = _value;
			set_value(_value + 1);
			return value;
		}
		dynamic_property operator --(int)
		{
			value_type value = _value;
			set_value(_value - 1);
			return value;
		}

		////////////////////////////////
		// Stream Extraction Operator //
		////////////////////////////////

		friend std::istream& operator >>(std::istream& in, dynamic_property& property)
		{
			value_type value;
			in >> value;
			property.set_value(value);
			return in;
		}
	private:
		mutator_type _mutator;
		value_type _value;
	};
}
