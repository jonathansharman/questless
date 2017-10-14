//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace ql
{
	//! Represents an inclusive, dynamic, constant, bounded range of numeric values, clamping values outside the range to the nearest valid value.
	//! @tparam NumericType A numeric type, supporting arithmetic operations.
	//! @tparam lower_bound The minimum value in the range (inclusive).
	//! @tparam upper_bound The maximum value in the range (inclusive).
	template <typename NumericType>
	class const_bounded
	{
	public:
		using numeric_type = NumericType;

		//! The minimum value in the range (inclusive).
		numeric_type const lower_bound;

		//! The maximum value in the range (inclusive).
		numeric_type const upper_bound;

		// Constructors

		constexpr const_bounded(numeric_type lower_bound, numeric_type upper_bound)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
		{}

		constexpr const_bounded(const_bounded const&) = default;

		constexpr const_bounded(const_bounded&& other)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
			, _number{bounded._number}
		{}

		constexpr const_bounded(numeric_type value, numeric_type lower_bound, numeric_type upper_bound)
			: lower_bound{lower_bound}
			, upper_bound{upper_bound}
			, _number{std::clamp(value, lower_bound, upper_bound)}
		{}

		// Accessors and Mutators

		//! Sets the contained value to the given new value, clamped to the valid range.
		void set(numeric_type const& value)
		{
			//! @todo When constexpr-if is available, use it to avoid upper bound check if upper_bound is the maximum type value.
			_number = std::clamp(value, lower_bound, upper_bound);
		}

		//! The contained value.
		constexpr operator numeric_type const&() const { return _number; }

		//! The contained value.
		constexpr numeric_type const& get() const { return _number; }

		// Asignment

		const_bounded& operator =(const_bounded const& bounded)
		{
			set(bounded._number);
			return *this;
		}
		const_bounded& operator =(numeric_type value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		const_bounded& operator +=(T const& that)
		{
			set(_number + that);
			return *this;
		}
		template <typename T>
		const_bounded& operator -=(T const& that)
		{
			set(_number - that);
			return *this;
		}
		template <typename T>
		const_bounded& operator *=(T const& that)
		{
			set(_number * that);
			return *this;
		}
		template <typename T>
		const_bounded& operator /=(T const& that)
		{
			set(_number / that);
			return *this;
		}
		template <typename T>
		const_bounded& operator %=(T const& that)
		{
			set(_number % that);
			return *this;
		}

		// Increment/decrement Operators

		const_bounded& operator ++()
		{
			set(_number + 1);
			return *this;
		}
		const_bounded& operator --()
		{
			set(_number - 1);
			return *this;
		}
		const_bounded operator ++(int)
		{
			numeric_type value = _number;
			set(_number + 1);
			return value;
		}
		const_bounded operator --(int)
		{
			numeric_type value = _number;
			set(_number - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, const_bounded& bounded)
		{
			numeric_type value;
			in >> value;
			bounded.set(value);
			return in;
		}
	private:
		numeric_type _number;
	};
}
