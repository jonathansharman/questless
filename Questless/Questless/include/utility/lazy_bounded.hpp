//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <istream>

namespace ql
{
	//! An inclusive range of arithmetic values bounded lazily to limits defined by function results.
	//! Values outside the range are clamped to the nearest valid value on read.
	//! @tparam ArithmeticType An arithmetic type, i.e. a type that supports arithmetic operations.
	//! @note See also @p static_bounded and @p dynamic_bounded.
	template <typename ArithmeticType>
	class lazy_bounded
	{
	public:
		using arithmetic_type = ArithmeticType;
		using bound_getter_type = std::function<arithmetic_type()>;

		//! The function for getting the inclusive minimum value in the range.
		std::function<arithmetic_type()> lower_bound_getter;
		//! The function for getting the inclusive maximum value in the range.
		std::function<arithmetic_type()> upper_bound_getter;

		//////////////////
		// Constructors //
		//////////////////

		constexpr lazy_bounded(bound_getter_type lower_bound_getter, bound_getter_type upper_bound_getter)
			: lower_bound_getter{std::move(lower_bound_getter)}
			, upper_bound_getter{std::move(upper_bound_getter)}
		{}

		constexpr lazy_bounded(arithmetic_type value, bound_getter_type lower_bound_getter, bound_getter_type upper_bound_getter)
			: lower_bound_getter{std::move(lower_bound_getter)}
			, upper_bound_getter{std::move(upper_bound_getter)}
			, _value{std::move(value)}
		{}

		constexpr lazy_bounded(lazy_bounded const&) = default;

		constexpr lazy_bounded(lazy_bounded&& other)
			: lower_bound_getter{std::move(other.lower_bound_getter)}
			, upper_bound_getter{std::move(other.upper_bound_getter)}
			, _value{std::move(bounded._value)}
		{}

		////////////////////////////
		// Accessors and Mutators //
		////////////////////////////

		//! The contained value.
		constexpr operator arithmetic_type() const { return value(); }

		//! The contained value.
		constexpr arithmetic_type value() const
		{
			auto result = std::clamp(_value, lower_bound_getter(), upper_bound_getter());
			return result;
		}

		//! Sets the contained value to the given new value, clamped to the valid range.
		void set_value(arithmetic_type value) { _value = std::move(value); }

		///////////////
		// Asignment //
		///////////////

		lazy_bounded& operator =(lazy_bounded const& bounded)
		{
			_value = bounded._value;
			return *this;
		}
		lazy_bounded& operator =(arithmetic_type value)
		{
			_value = std::move(value);
			return *this;
		}

		template <typename T>
		lazy_bounded& operator +=(T const& that)
		{
			_value = value() + that;
			return *this;
		}
		template <typename T>
		lazy_bounded& operator -=(T const& that)
		{
			_value = value() - that;
			return *this;
		}
		template <typename T>
		lazy_bounded& operator *=(T const& that)
		{
			_value = value() * that;
			return *this;
		}
		template <typename T>
		lazy_bounded& operator /=(T const& that)
		{
			_value = value() / that;
			return *this;
		}
		template <typename T>
		lazy_bounded& operator %=(T const& that)
		{
			_value = value() % that;
			return *this;
		}

		///////////////////////////////////
		// Increment/decrement Operators //
		///////////////////////////////////

		lazy_bounded& operator ++()
		{
			_value = value() + 1;
			return *this;
		}
		lazy_bounded& operator --()
		{
			_value = value() - 1;
			return *this;
		}
		lazy_bounded operator ++(int)
		{
			arithmetic_type old_value = value();
			_value = old_value + 1;
			return old_value;
		}
		lazy_bounded operator --(int)
		{
			arithmetic_type old_value = value();
			_value = old_value - 1;
			return old_value;
		}

		////////////////////////////////
		// Stream Extraction Operator //
		////////////////////////////////

		friend std::istream& operator >>(std::istream& in, lazy_bounded& bounded)
		{
			in >> bounded._value;
			return in;
		}
	private:
		arithmetic_type _value;
	};
}

//! @todo Tests.
