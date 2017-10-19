//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace ql
{
	//! An inclusive, dynamically bounded range of arithmetic values, clamping values outside the range to the nearest valid value.
	//! @tparam ArithmeticType An arithmetic type, i.e. a type that supports arithmetic operations.
	//! @note See also @p static_bounded and @p lazy_bounded.
	template <typename ArithmeticType, typename = std::enable_if_t<std::is_arithmetic_v<ArithmeticType>>>
	class dynamic_bounded
	{
	public:
		using arithmetic_type = ArithmeticType;

		//////////////////
		// Constructors //
		//////////////////

		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		constexpr dynamic_bounded(arithmetic_type lower_bound, arithmetic_type upper_bound)
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::move(upper_bound)}
		{}

		//! @param value The initial value, which will be clamped to within the range.
		//! @param lower_bound The minimum value in the range (inclusive).
		//! @param upper_bound The maximum value in the range (inclusive).
		constexpr dynamic_bounded(arithmetic_type value, arithmetic_type lower_bound, arithmetic_type upper_bound)
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::move(upper_bound)}
			, _value{std::clamp(value, lower_bound, upper_bound)}
		{}

		constexpr dynamic_bounded(dynamic_bounded const&) = default;

		constexpr dynamic_bounded(dynamic_bounded&& other)
			: _lower_bound{std::move(lower_bound)}
			, _upper_bound{std::move(upper_bound)}
			, _value{std::move(bounded._value)}
		{}

		////////////////////////////
		// Accessors and Mutators //
		////////////////////////////

		//! The contained value.
		constexpr operator arithmetic_type const&() const { return _value; }

		//! The contained value.
		constexpr arithmetic_type const& value() const { return _value; }

		//! The minimum value in the range (inclusive).
		constexpr arithmetic_type const& lower_bound() const { return _lower_bound; }

		//! The maximum value in the range (inclusive).
		constexpr arithmetic_type const& upper_bound() const { return _upper_bound; }

		//! Sets the contained value to the given new value, clamped to the valid range.
		void set_value(arithmetic_type const& value)
		{
			//! @todo When constexpr-if is available, use it to avoid upper bound check if upper_bound is the maximum type value.
			_value = std::clamp(value, _lower_bound, _upper_bound);
		}

		//! Sets the inclusive minimum value in the range to @p lower_bound and clamps the contained value if necessary.
		void set_lower_bound(arithmetic_type lower_bound)
		{
			_lower_bound = std::move(lower_bound);
			if (_value < _lower_bound) {
				_value = _lower_bound;
			}
		}

		//! Sets the inclusive maximum value in the range to @p upper_bound and clamps the contained value if necessary.
		void set_upper_bound(arithmetic_type upper_bound)
		{
			_upper_bound = std::move(upper_bound);
			if (_value > _upper_bound) {
				_value = _upper_bound;
			}
		}

		///////////////
		// Asignment //
		///////////////

		dynamic_bounded& operator =(dynamic_bounded const& bounded)
		{
			set_value(bounded._value);
			return *this;
		}
		dynamic_bounded& operator =(arithmetic_type value)
		{
			set_value(std::move(value));
			return *this;
		}

		template <typename T>
		dynamic_bounded& operator +=(T const& that)
		{
			set_value(_value + that);
			return *this;
		}
		template <typename T>
		dynamic_bounded& operator -=(T const& that)
		{
			set_value(_value - that);
			return *this;
		}
		template <typename T>
		dynamic_bounded& operator *=(T const& that)
		{
			set_value(_value * that);
			return *this;
		}
		template <typename T>
		dynamic_bounded& operator /=(T const& that)
		{
			set_value(_value / that);
			return *this;
		}
		template <typename T>
		dynamic_bounded& operator %=(T const& that)
		{
			set_value(_value % that);
			return *this;
		}

		///////////////////////////////////
		// Increment/decrement Operators //
		///////////////////////////////////

		dynamic_bounded& operator ++()
		{
			set_value(_value + 1);
			return *this;
		}
		dynamic_bounded& operator --()
		{
			set_value(_value - 1);
			return *this;
		}
		dynamic_bounded operator ++(int)
		{
			arithmetic_type value = _value;
			set_value(_value + 1);
			return value;
		}
		dynamic_bounded operator --(int)
		{
			arithmetic_type value = _value;
			set_value(_value - 1);
			return value;
		}

		////////////////////////////////
		// Stream Extraction Operator //
		////////////////////////////////

		friend std::istream& operator >>(std::istream& in, dynamic_bounded& bounded)
		{
			arithmetic_type value;
			in >> value;
			bounded.set_value(value);
			return in;
		}
	private:
		arithmetic_type const _lower_bound;
		arithmetic_type const _upper_bound;
		arithmetic_type _value;
	};
}
