//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "unit.hpp"

namespace meta {
	//! Represents a quantity with the given data representation and unit.
	template <typename Rep, typename Unit>
	struct quantity {
		//! An alias to this type.
		using this_type = quantity<Rep, Unit>;
		//! The data representation of this quantity type.
		using rep = Rep;
		//! The unit of this quantity type.
		using unit = Unit;

		//! The underlying unit-less value.
		rep value;

		//! Constructs a quantity from the default value of the underlying type.
		constexpr quantity() = default;

		//! Explicitly constructs a quantity from a value.
		explicit constexpr quantity(rep const& value) : value(value) {}

		//! Explicitly constructs a quantity from a value.
		explicit constexpr quantity(rep&& value) : value(std::move(value)) {}

		//! Construct from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr quantity(quantity<rep, ThatUnit> const& that) : value{that.value} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! Construct from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr quantity(quantity<rep, ThatUnit>&& that) : value{std::move(that.value)} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! Allow implicit casting of a unitless quantity to the representation type.
		template <typename = std::enable_if_t<std::is_same_v<unit, empty>>>
		constexpr operator rep() const {
			return value;
		}

		template <typename ThatUnit>
		constexpr bool operator ==(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value == that.value;
		}
		template <typename ThatUnit>
		constexpr bool operator !=(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value != that.value;
		}
		template <typename ThatUnit>
		constexpr bool operator <(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value < that.value;
		}
		template <typename ThatUnit>
		constexpr bool operator <=(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value <= that.value;
		}
		template <typename ThatUnit>
		constexpr bool operator >(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value > that.value;
		}
		template <typename ThatUnit>
		constexpr bool operator >=(quantity<rep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to compare quantities with different units.");
			return value >= that.value;
		}

		//! Assign from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr auto& operator =(quantity<rep, ThatUnit> const& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign quantities of different units.");
			value = that.value;
			return *this;
		};

		//! Assign from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr auto& operator =(quantity<rep, ThatUnit>&& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign quantities of different units.");
			value = std::move(that.value);
			return *this;
		};

		//! Add an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr auto& operator +=(quantity<rep, ThatUnit> const& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to add quantities of different units.");
			value += that.value;
			return *this;
		}

		//! Subtract an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr auto& operator -=(quantity<rep, ThatUnit> const& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to subtract quantities of different units.");
			value -= that.value;
			return *this;
		}

		//! Multiply by a scalar.
		constexpr auto& operator *=(rep const& k) {
			value *= k;
			return *this;
		}

		//! Divide by a scalar.
		constexpr auto& operator /=(rep const& k) {
			value /= k;
			return *this;
		}

		//! Add two quantities of the same representation and unit.
		template <typename ThatUnit>
		friend constexpr auto operator +(this_type const& q1, quantity<rep, ThatUnit> const& q2) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to add quantities of different units.");
			return this_type{q1.value + q2.value};
		}

		//! Negates this quantity.
		constexpr auto operator -() {
			return this_type{-value};
		}

		//! Subtract two quantities of the same representation and unit.
		template <typename ThatUnit>
		friend constexpr auto operator -(this_type const& q1, quantity<rep, ThatUnit> const& q2) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to subtract quantities of different units.");
			return this_type{q1.value - q2.value};
		}

		//! Multiply a quantity by a scalar.
		friend constexpr auto operator *(this_type const& q, rep const& k) {
			return this_type{q.value * k};
		}
		//! Multiply a quantity by a scalar.
		friend constexpr auto operator *(rep const& k, this_type const& q) {
			return this_type{k * q.value};
		}

		//! Divide a quantity by a scalar.
		friend constexpr auto operator /(this_type const& q, rep const& k) {
			return this_type{q.value / k};
		}

		//! Divide a scalar by a quantity.
		friend constexpr auto operator /(rep const& k, this_type const& q) {
			return quantity<rep, inverse_t<unit>>{k / q.value};
		}
	};

	//! Multiply two quantities.
	template <typename Rep, typename Unit1, typename Unit2>
	constexpr auto operator *(quantity<Rep, Unit1> const& q1, quantity<Rep, Unit2> const& q2) {
		return quantity<Rep, product_t<Unit1, Unit2>>{q1.value * q2.value};
	}

	//! Divide two quantities.
	template <typename Rep, typename Unit1, typename Unit2>
	constexpr auto operator /(quantity<Rep, Unit1> const& q1, quantity<Rep, Unit2> const& q2) {
		return quantity<Rep, quotient_t<Unit1, Unit2>>{q1.value / q2.value};
	}

	//namespace detail {
	//	//! Extend product to quantities.
	//	template <typename Rep, typename Unit1, typename Unit2>
	//	struct detail::product<quantity<Rep, Unit1>, quantity<Rep, Unit2>> {
	//		using type = quantity<Rep, detail::product_t<Unit1, Unit2>>;
	//	};

	//	//! Extend inverse to quantities.
	//	template <typename Rep, typename Unit>
	//	struct detail::inverse<quantity<Rep, Unit>> {
	//		using type = quantity<Rep, detail::inverse_t<Unit>>;
	//	};

	//	//! Extend quotient to quantities.
	//	template <typename Rep, typename Unit1, typename Unit2>
	//	struct detail::quotient<quantity<Rep, Unit1>, quantity<Rep, Unit2>> {
	//		using type = quantity<Rep, detail::quotient_t<Unit1, Unit2>>;
	//	};
	//}
}
