//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace questless
{
	//! Represents an inclusive, bounded range of numeric values, clamping values outside the range to the nearest valid value.
	//! @tparam NumericType A numeric type, supporting arithmetic operations.
	//! @tparam lower_bound The minimum value in the range (inclusive).
	//! @tparam upper_bound The maximum value in the range (inclusive).
	template <typename NumericType>
	class DynamicBounded
	{
	public:
		using numeric_t = NumericType;

		//! The minimum value in the range (inclusive).
		numeric_t const lower_bound;

		//! The maximum value in the range (inclusive).
		numeric_t const upper_bound;

		// Constructors

		constexpr DynamicBounded(numeric_t lower_bound, numeric_t upper_bound)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
		{}

		constexpr DynamicBounded(DynamicBounded const&) = default;

		constexpr DynamicBounded(DynamicBounded&& other)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
			, _number{bounded._number}
		{}

		constexpr DynamicBounded(numeric_t value, numeric_t lower_bound, numeric_t upper_bound)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
			, _number{std::clamp(value, lower_bound, upper_bound)}
		{}

		// Accessors and Mutators

		//! Sets the contained value to the given new value, clamped to the valid range.
		void set(numeric_t const& value)
		{
			//! @todo When constexpr-if is available, use it to avoid upper bound check if upper_bound is the maximum type value.
			_number = std::clamp(value, lower_bound, upper_bound);
		}

		//! The contained value.
		constexpr operator numeric_t const&() const { return _number; }

		//! The contained value.
		constexpr numeric_t const& get() const { return _number; }

		// Asignment

		DynamicBounded& operator =(DynamicBounded const& bounded)
		{
			set(bounded._number);
			return *this;
		}
		DynamicBounded& operator =(numeric_t value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		DynamicBounded& operator +=(T const& that)
		{
			set(_number + that);
			return *this;
		}
		template <typename T>
		DynamicBounded& operator -=(T const& that)
		{
			set(_number - that);
			return *this;
		}
		template <typename T>
		DynamicBounded& operator *=(T const& that)
		{
			set(_number * that);
			return *this;
		}
		template <typename T>
		DynamicBounded& operator /=(T const& that)
		{
			set(_number / that);
			return *this;
		}
		template <typename T>
		DynamicBounded& operator %=(T const& that)
		{
			set(_number % that);
			return *this;
		}

		// Increment/decrement Operators

		DynamicBounded& operator ++()
		{
			set(_number + 1);
			return *this;
		}
		DynamicBounded& operator --()
		{
			set(_number - 1);
			return *this;
		}
		DynamicBounded operator ++(int)
		{
			numeric_t value = _number;
			set(_number + 1);
			return value;
		}
		DynamicBounded operator --(int)
		{
			numeric_t value = _number;
			set(_number - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, DynamicBounded& bounded)
		{
			numeric_t value;
			in >> value;
			bounded.set(value);
			return in;
		}
	private:
		numeric_t _number;
	};
}
