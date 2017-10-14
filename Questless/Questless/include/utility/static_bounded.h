//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace ql
{
	namespace detail
	{
		template <typename NumericType>
		struct maximum {};

		template <>
		struct maximum<int>
		{
			static constexpr int value = ::std::numeric_limits<int>::max();
		};
		template <>
		struct maximum<double>
		{
			static constexpr double value = ::std::numeric_limits<double>::max();
		};
	}

	//! Represents an inclusive, compile-time-bounded range of numeric values, clamping values outside the range to the nearest valid value.
	//! @tparam NumericType A numeric type, supporting arithmetic operations.
	//! @tparam LowerBound The minimum value in the range (inclusive).
	//! @tparam UpperBound The maximum value in the range (inclusive).
	template
		< typename NumericType
		, typename NumericType const& LowerBound
		, typename NumericType const& UpperBound = detail::maximum<NumericType>::value
		>
	class static_bounded
	{
	public:
		using numeric_type = NumericType;

		//! The minimum value in the range (inclusive).
		static constexpr numeric_type lower_bound = LowerBound;

		//! The maximum value in the range (inclusive).
		static constexpr numeric_type upper_bound = UpperBound;

		// Constructors

		constexpr static_bounded() = default;

		constexpr static_bounded(static_bounded const&) = default;
		constexpr static_bounded(static_bounded&&) = default;

		constexpr static_bounded(numeric_type value) : _number{std::clamp(value, lower_bound, upper_bound)} {}

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

		static_bounded& operator =(static_bounded const& bounded)
		{
			set(bounded._number);
			return *this;
		}
		static_bounded& operator =(numeric_type value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		static_bounded& operator +=(T const& that)
		{
			set(_number + that);
			return *this;
		}
		template <typename T>
		static_bounded& operator -=(T const& that)
		{
			set(_number - that);
			return *this;
		}
		template <typename T>
		static_bounded& operator *=(T const& that)
		{
			set(_number * that);
			return *this;
		}
		template <typename T>
		static_bounded& operator /=(T const& that)
		{
			set(_number / that);
			return *this;
		}
		template <typename T>
		static_bounded& operator %=(T const& that)
		{
			set(_number % that);
			return *this;
		}

		// Increment/decrement Operators

		static_bounded& operator ++()
		{
			set(_number + 1);
			return *this;
		}
		static_bounded& operator --()
		{
			set(_number - 1);
			return *this;
		}
		static_bounded operator ++(int)
		{
			numeric_type value = _number;
			set(_number + 1);
			return value;
		}
		static_bounded operator --(int)
		{
			numeric_type value = _number;
			set(_number - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, static_bounded& bounded)
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
