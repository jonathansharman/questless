//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace questless
{
	namespace detail
	{
		template <typename NumericType>
		struct Maximum {};

		template <>
		struct Maximum<int>
		{
			static constexpr int value = ::std::numeric_limits<int>::max();
		};
		template <>
		struct Maximum<double>
		{
			static constexpr double value = ::std::numeric_limits<double>::max();
		};
	}

	//! Represents an inclusive, bounded range of numeric values, clamping values outside the range to the nearest valid value.
	//! @tparam NumericType A numeric type, supporting arithmetic operations.
	//! @tparam lower_bound The minimum value in the range (inclusive).
	//! @tparam upper_bound The maximum value in the range (inclusive).
	template
		< typename NumericType
		, typename NumericType const& lower_bound
		, typename NumericType const& upper_bound = detail::Maximum<NumericType>::value
		>
	class Bounded
	{
	public:
		using numeric_t = NumericType;

		// Constructors

		constexpr Bounded() = default;

		constexpr Bounded(Bounded const& bounded) = default;
		constexpr Bounded(Bounded&& bounded) = default;

		constexpr Bounded(numeric_t value) : _number{std::clamp(value, lower_bound, upper_bound)} {}

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

		Bounded& operator =(Bounded const& bounded)
		{
			set(bounded._number);
			return *this;
		}
		Bounded& operator =(numeric_t value)
		{
			set(value);
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		Bounded& operator +=(T const& that)
		{
			set(_number + that);
			return *this;
		}
		template <typename T>
		Bounded& operator -=(T const& that)
		{
			set(_number - that);
			return *this;
		}
		template <typename T>
		Bounded& operator *=(T const& that)
		{
			set(_number * that);
			return *this;
		}
		template <typename T>
		Bounded& operator /=(T const& that)
		{
			set(_number / that);
			return *this;
		}
		template <typename T>
		Bounded& operator %=(T const& that)
		{
			set(_number % that);
			return *this;
		}

		// Increment/decrement Operators

		Bounded& operator ++()
		{
			set(_number + 1);
			return *this;
		}
		Bounded& operator --()
		{
			set(_number - 1);
			return *this;
		}
		Bounded operator ++(int)
		{
			numeric_t value = _number;
			set(_number + 1);
			return value;
		}
		Bounded operator --(int)
		{
			numeric_t value = _number;
			set(_number - 1);
			return value;
		}

		// Stream Extraction Operator

		friend std::istream& operator >>(std::istream& in, Bounded& bounded)
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
