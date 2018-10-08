//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cereal/cereal.hpp>

namespace ql {
	//! CRTP base class for bounded types (@p static_bounded, @p Derived, and @p dynamic_bounded). Cannot be used directly.
	//! @tparam ArithmeticType The underlying type.
	template <typename ArithmeticType, typename Derived>
	class bounded {
	public:
		using arithmetic_type = ArithmeticType;

		template <typename Archive>
		void save(Archive& archive) const {
			archive(cereal::make_nvp("value", Derived::value()));
		}

		template <typename Archive>
		void load(Archive& archive) {
			arithmetic_type value;
			archive(cereal::make_nvp("value", value));
			set_value(std::move(value));
		}

		//! The contained value.
		constexpr operator arithmetic_type() const { return Derived::value(); }

		// Asignment Operators

		Derived& operator =(Derived const& bounded) {
			set_value(bounded.value());
			return *this;
		}
		Derived& operator =(Derived&& bounded) {
			set_value(std::move(bounded.value()));
			return *this;
		}
		Derived& operator =(arithmetic_type const& value) {
			set_value(value);
			return *this;
		}
		Derived& operator =(arithmetic_type&& value) {
			set_value(std::move(value));
			return *this;
		}

		// Arithmetic Assignment Operators

		template <typename T>
		Derived& operator +=(T const& that) {
			set_value(Derived::value() + that);
			return *this;
		}
		template <typename T>
		Derived& operator -=(T const& that) {
			set_value(Derived::value() - that);
			return *this;
		}
		template <typename T>
		Derived& operator *=(T const& that) {
			set_value(Derived::value() * that);
			return *this;
		}
		template <typename T>
		Derived& operator /=(T const& that) {
			set_value(Derived::value() / that);
			return *this;
		}
		template <typename T>
		Derived& operator %=(T const& that) {
			set_value(Derived::value() % that);
			return *this;
		}
		template <typename T>
		Derived& operator >>=(int shift) {
			set_value(Derived::value() >> shift);
			return *this;
		}
		template <typename T>
		Derived& operator <<=(int shift) {
			set_value(Derived::value() << shift);
			return *this;
		}

		// Increment/decrement Operators

		Derived& operator ++() {
			set_value(Derived::value() + 1);
			return *this;
		}
		Derived& operator --() {
			set_value(Derived::value() - 1);
			return *this;
		}
		Derived operator ++(int) {
			arithmetic_type old_value = Derived::value();
			set_value(Derived::value() + 1);
			return old_value;
		}
		Derived operator --(int) {
			arithmetic_type old_value = Derived::value();
			set_value(Derived::value() - 1);
			return old_value;
		}

		// Comparison Operators

		template <typename T>
		constexpr bool operator ==(T const& that) const { return Derived::value() == that; }
		constexpr bool operator ==(Derived const& that) const { return Derived::value() == that.value(); }

		template <typename T>
		constexpr bool operator !=(T const& that) const { return Derived::value() != that; }
		constexpr bool operator !=(Derived const& that) const { return Derived::value() != that.value(); }

		template <typename T>
		constexpr bool operator <(T const& that) const { return Derived::value() < that; }
		constexpr bool operator <(Derived const& that) const { return Derived::value() < that.value(); }

		template <typename T>
		constexpr bool operator <=(T const& that) const { return Derived::value() <= that; }
		constexpr bool operator <=(Derived const& that) const { return Derived::value() <= that.value(); }

		template <typename T>
		constexpr bool operator >(T const& that) const { return Derived::value() > that; }
		constexpr bool operator >(Derived const& that) const { return Derived::value() > that.value(); }

		template <typename T>
		constexpr bool operator >=(T const& that) const { return Derived::value() >= that; }
		constexpr bool operator >=(Derived const& that) const { return Derived::value() >= that.value(); }

		// Arithmetic Operators

		template <typename T>
		Derived operator +(T const& that) const { return Derived::value() + that; }
		Derived operator +(Derived const& that) const { return Derived::value() + that.value(); }

		Derived operator -() const { return -Derived::value(); }

		template <typename T>
		Derived operator -(T const& that) const { return Derived::value() - that; }
		Derived operator -(Derived const& that) const { return Derived::value() - that.value(); }

		template <typename T>
		Derived operator *(T const& that) const { return Derived::value() * that; }
		Derived operator *(Derived const& that) const { return Derived::value() * that.value(); }

		template <typename T>
		Derived operator /(T const& that) const { return Derived::value() / that; }
		Derived operator /(Derived const& that) const { return Derived::value() / that.value(); }

		template <typename T>
		Derived operator %(T const& that) const { return Derived::value() % that; }
		Derived operator %(Derived const& that) const { return Derived::value() % that.value(); }

		Derived operator >>(int shift) const { return Derived::value() >> shift; }
		Derived operator <<(int shift) const { return Derived::value() << shift; }
	protected:
		constexpr bounded() = default;
	};
}
