//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace ql {
	namespace detail {
		template <typename ArithmeticType>
		struct maximum {};

		template <>
		struct maximum<int> {
			static constexpr int value = ::std::numeric_limits<int>::max();
		};
		template <>
		struct maximum<double> {
			static constexpr double value = ::std::numeric_limits<double>::max();
		};
	}

	//! An inclusive, statically bounded range of arithmetic values, clamping values outside the range to the nearest valid value.
	//! @tparam ArithmeticType An arithmetic type, i.e. a type that supports arithmetic operations.
	//! @tparam LowerBound The minimum value in the range (inclusive).
	//! @tparam UpperBound The maximum value in the range (inclusive).
	//! @note See also @p dynamic_bounded and @p lazy_bounded.
	template
		< typename ArithmeticType
		, typename ArithmeticType const& LowerBound
		, typename ArithmeticType const& UpperBound = detail::maximum<ArithmeticType>::value
		>
	class static_bounded {
	public:
		using arithmetic_type = ArithmeticType;

		//! The minimum value in the range (inclusive).
		static constexpr arithmetic_type lower_bound = LowerBound;

		//! The maximum value in the range (inclusive).
		static constexpr arithmetic_type upper_bound = UpperBound;

		//////////////////
		// Constructors //
		//////////////////

		constexpr static_bounded() = default;

		constexpr static_bounded(static_bounded const&) = default;
		constexpr static_bounded(static_bounded&&) = default;

		constexpr static_bounded(arithmetic_type value) : _value{std::clamp(value, lower_bound, upper_bound)} {}

		////////////////////////////
		// Accessors and Mutators //
		////////////////////////////

		//! The contained value.
		constexpr operator arithmetic_type const&() const { return _value; }

		//! The contained value.
		constexpr arithmetic_type const& value() const { return _value; }

		//! Sets the contained value to the given new value, clamped to the valid range.
		void set_value(arithmetic_type const& value) {
			constexpr auto min = std::numeric_limits<arithmetic_type>::min();
			constexpr auto max = std::numeric_limits<arithmetic_type>::max();
			if constexpr (lower_bound == min && upper_bound == max) {
				// No checks.
				_value = value; 
			} else if constexpr (lower_bound != min && upper_bound == max) {
				// Only check lower bound.
				_value = std::max(value, lower_bound);
			} else if constexpr (lower_bound == min && upper_bound != max) {
				// Only check upper bound.
				_value = std::min(value, upper_bound);
			} else {
				// Check both bounds.
				_value = std::clamp(value, lower_bound, upper_bound);
			}
		}

		///////////////
		// Asignment //
		///////////////

		static_bounded& operator =(static_bounded const& bounded) {
			set_value(bounded._value);
			return *this;
		}
		static_bounded& operator =(arithmetic_type const& value) {
			set_value(std::move(value));
			return *this;
		}

		template <typename T>
		static_bounded& operator +=(T const& that) {
			set_value(_value + that);
			return *this;
		}
		template <typename T>
		static_bounded& operator -=(T const& that) {
			set_value(_value - that);
			return *this;
		}
		template <typename T>
		static_bounded& operator *=(T const& that) {
			set_value(_value * that);
			return *this;
		}
		template <typename T>
		static_bounded& operator /=(T const& that) {
			set_value(_value / that);
			return *this;
		}
		template <typename T>
		static_bounded& operator %=(T const& that) {
			set_value(_value % that);
			return *this;
		}

		///////////////////////////////////
		// Increment/decrement Operators //
		///////////////////////////////////

		static_bounded& operator ++() {
			set_value(_value + 1);
			return *this;
		}
		static_bounded& operator --() {
			set_value(_value - 1);
			return *this;
		}
		static_bounded operator ++(int) {
			arithmetic_type value = _value;
			set_value(_value + 1);
			return value;
		}
		static_bounded operator --(int) {
			arithmetic_type value = _value;
			set_value(_value - 1);
			return value;
		}

		////////////////////////////////
		// Stream Extraction Operator //
		////////////////////////////////

		friend std::istream& operator >>(std::istream& in, static_bounded& bounded) {
			arithmetic_type value;
			in >> value;
			bounded.set_value(value);
			return in;
		}
	private:
		arithmetic_type _value;
	};
}
