//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <limits>

namespace ql {
	//! Wraps an arithmetic type, clamping negative values to zero.
	template <typename ArithmeticType>
	class nonnegative {
	public:
		using arithmetic_type = ArithmeticType;

		static constexpr arithmetic_type zero{0};

		constexpr nonnegative() = default;

		constexpr nonnegative(nonnegative const&) = default;
		constexpr nonnegative(nonnegative&&) = default;

		constexpr nonnegative(arithmetic_type const& value) : _value{std::max(zero, value)} {}

		//! The contained value.
		constexpr operator arithmetic_type const&() const { return _value; }

		//! The contained value.
		constexpr arithmetic_type const& value() const { return _value; }

		//! Sets the contained value to the given new value, clamped to the valid range.
		constexpr void set_value(arithmetic_type const& value) {
			_value = std::max(zero, value);
		}

		constexpr nonnegative& operator =(nonnegative const& bounded) {
			set_value(bounded._value);
			return *this;
		}

		constexpr nonnegative& operator =(arithmetic_type const& value) {
			set_value(std::move(value));
			return *this;
		}

		template <typename T>
		constexpr nonnegative& operator +=(T const& that) {
			set_value(_value + that);
			return *this;
		}
		constexpr nonnegative& operator +=(nonnegative const& that) {
			set_value(_value + that._value);
			return *this;
		}

		template <typename T>
		constexpr nonnegative& operator -=(T const& that) {
			set_value(_value - that);
			return *this;
		}
		constexpr nonnegative& operator -=(nonnegative const& that) {
			set_value(_value - that._value);
			return *this;
		}

		template <typename T>
		constexpr nonnegative& operator *=(T const& that) {
			set_value(_value * that);
			return *this;
		}
		constexpr nonnegative& operator *=(nonnegative const& that) {
			set_value(_value * that._value);
			return *this;
		}

		template <typename T>
		constexpr nonnegative& operator /=(T const& that) {
			set_value(_value / that);
			return *this;
		}
		constexpr nonnegative& operator /=(nonnegative const& that) {
			set_value(_value / that._value);
			return *this;
		}

		template <typename T>
		constexpr nonnegative& operator %=(T const& that) {
			set_value(_value % that);
			return *this;
		}
		constexpr nonnegative& operator %=(nonnegative const& that) {
			set_value(_value % that._value);
			return *this;
		}

		constexpr nonnegative& operator ++() {
			set_value(_value + 1);
			return *this;
		}

		constexpr nonnegative& operator --() {
			set_value(_value - 1);
			return *this;
		}

		constexpr nonnegative operator ++(int) {
			arithmetic_type value = _value;
			set_value(_value + 1);
			return value;
		}

		constexpr nonnegative operator --(int) {
			arithmetic_type value = _value;
			set_value(_value - 1);
			return value;
		}

		template <typename T>
		constexpr bool operator ==(T const& that) const { return _value == that; }
		constexpr bool operator ==(nonnegative const& that) const { return _value == that._value; }

		template <typename T>
		constexpr bool operator !=(T const& that) const { return _value != that; }
		constexpr bool operator !=(nonnegative const& that) const { return _value != that._value; }

		template <typename T>
		constexpr bool operator <(T const& that) const { return _value < that; }
		constexpr bool operator <(nonnegative const& that) const { return _value < that._value; }

		template <typename T>
		constexpr bool operator <=(T const& that) const { return _value <= that; }
		constexpr bool operator <=(nonnegative const& that) const { return _value <= that._value; }

		template <typename T>
		constexpr bool operator >(T const& that) const { return _value > that; }
		constexpr bool operator >(nonnegative const& that) const { return _value > that._value; }

		template <typename T>
		constexpr bool operator >=(T const& that) const { return _value >= that; }
		constexpr bool operator >=(nonnegative const& that) const { return _value >= that._value; }

		template <typename T>
		constexpr nonnegative operator +(T const& that) const { return nonnegative{_value + that}; }
		constexpr nonnegative operator +(nonnegative const& that) const { return nonnegative{_value + that._value}; }

		constexpr nonnegative operator -() const { return nonnegative{-_value}; }

		template <typename T>
		constexpr nonnegative operator -(T const& that) const { return nonnegative{_value - that}; }
		constexpr nonnegative operator -(nonnegative const& that) const { return nonnegative{_value - that._value}; }

		template <typename T>
		constexpr nonnegative operator *(T const& that) const { return nonnegative{_value * that}; }
		constexpr nonnegative operator *(nonnegative const& that) const { return nonnegative{_value * that._value}; }

		template <typename T>
		constexpr nonnegative operator /(T const& that) const { return nonnegative{_value * that}; }
		constexpr nonnegative operator /(nonnegative const& that) const { return nonnegative{_value * that._value}; }
		
		template <typename T>
		constexpr nonnegative operator %(T const& that) const { return nonnegative{_value & that}; }
		constexpr nonnegative operator %(nonnegative const& that) const { return nonnegative{_value & that._value}; }

		friend std::istream& operator >>(std::istream& in, nonnegative& bounded) {
			arithmetic_type value;
			in >> value;
			bounded.set_value(value);
			return in;
		}
	private:
		arithmetic_type _value;
	};

	// Literals

	constexpr nonnegative<int> operator "" _nn(unsigned long long value) {
		return nonnegative<int>{static_cast<int>(value)};
	}

	constexpr nonnegative<double> operator "" _nn(long double value) {
		return nonnegative<double>{static_cast<double>(value)};
	}
}
