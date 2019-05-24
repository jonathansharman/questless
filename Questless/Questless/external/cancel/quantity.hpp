//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "unit.hpp"

#include <gcem.hpp>

#include <cmath>
#include <utility>

namespace cancel {
	//! Represents a quantity with the given data representation and unit.
	template <typename Rep, typename Unit>
	struct quantity {
		//! The data representation of this quantity type.
		using rep = Rep;
		//! The unit of this quantity type.
		using unit = Unit;

		//! The underlying value of this quantity.
		rep value;

		//! Constructs a quantity from the default value of the underlying type.
		constexpr quantity() noexcept(std::is_nothrow_default_constructible_v<rep>) = default;

		//! Explicitly copy-constructs a quantity from a value.
		explicit constexpr quantity(rep const& value) noexcept(std::is_nothrow_copy_constructible_v<rep>) : value{value} {}

		//! Explicitly move-constructs a quantity from a value.
		explicit constexpr quantity(rep&& value) noexcept(std::is_nothrow_move_constructible_v<rep>) : value{std::move(value)} {}

		//! Copy-constructs a quantity from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr quantity(quantity<rep, ThatUnit> const& that) noexcept(std::is_nothrow_copy_constructible_v<rep>) : value{that.value} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! Move-constructs a quantity from an object of the same representation and unit.
		template <typename ThatUnit>
		constexpr quantity(quantity<rep, ThatUnit>&& that) noexcept(std::is_nothrow_move_constructible_v<rep>) : value{std::move(that.value)} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! cereal-compatible save function.
		template <typename Archive>
		void save(Archive& archive) const { archive(value); }

		//! cereal-compatible load function.
		template <typename Archive>
		void load(Archive& archive) { archive(value); }

		//! Copy-assign from an object of the same representation and unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator =(quantity<ThatRep, ThatUnit> const& that) noexcept(std::is_nothrow_copy_assignable_v<rep>) {
			static_assert(std::is_convertible_v<ThatRep, rep>, "Attempted to assign from a quantity with an incompatible representation.");
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign from a quantity of a different unit.");
			value = static_cast<rep>(that.value);
			return *this;
		};

		//! Move-assign from an object of the same representation and unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator =(quantity<ThatRep, ThatUnit>&& that) noexcept(std::is_nothrow_move_assignable_v<rep>) {
			static_assert(std::is_convertible_v<ThatRep, rep>, "Attempted to assign from a quantity with an incompatible representation.");
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign from a quantity of a different unit.");
			value = std::move(static_cast<rep>(that.value));
			return *this;
		};

		//! Add a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator +=(quantity<ThatRep, ThatUnit> const& that) {
			static_assert(std::is_convertible_v<ThatRep, rep>, "Attempted to add a quantity with an incompatible representation.");
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to add a quantity of a different unit.");
			value += static_cast<rep>(that.value);
			return *this;
		}

		//! Subtract a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator -=(quantity<ThatRep, ThatUnit> const& that) {
			static_assert(std::is_convertible_v<ThatRep, rep>, "Attempted to substract a quantity with an incompatible representation.");
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to subtract a quantity of a different unit.");
			value -= static_cast<rep>(that.value);
			return *this;
		}

		//! Multiply by a scalar.
		template <typename K>
		constexpr auto& operator *=(K const& k) {
			static_assert(std::is_convertible_v<K, rep>, "Argument is not convertible to quantity's representation type.");
			value *= static_cast<rep>(k);
			return *this;
		}
		//! Multiply by a unitless quantity.
		template <typename K>
		constexpr auto& operator *=(quantity<K, unit_t<>> const& k) {
			static_assert(std::is_convertible_v<K, rep>, "Argument is not convertible to quantity's representation type.");
			value *= static_cast<rep>(k.value);
			return *this;
		}

		//! Divide by a scalar.
		template <typename K>
		constexpr auto& operator /=(K const& k) {
			static_assert(std::is_convertible_v<K, rep>, "Argument is not convertible to quantity's representation type.");
			value /= static_cast<rep>(k);
			return *this;
		}
		//! Divide by a unitless quantity.
		template <typename K>
		constexpr auto& operator /=(quantity<K, unit_t<>> const& k) {
			static_assert(std::is_convertible_v<K, rep>, "Argument is not convertible to quantity's representation type.");
			value /= static_cast<rep>(k.value);
			return *this;
		}

		//! Mod by a scalar.
		template <typename K>
		constexpr auto& operator %=(K const& k) {
			static_assert(std::is_convertible_v<K, rep>, "Argument is not convertible to quantity's representation type.");
			value %= static_cast<rep>(k);
			return *this;
		}

		//! Mod by a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator %=(quantity<ThatRep, ThatUnit> const& that) const {
			static_assert(std::is_convertible_v<ThatRep, rep>, "Attempted to mod by a quantity with an incompatible representation.");
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to mod by a quantity of a different unit.");
			value %= static_cast<rep>(that.value);
			return *this;
		}

		//! Pre-increment.
		constexpr auto& operator ++() {
			++value;
			return *this;
		}

		//! Post-increment.
		constexpr auto operator ++(int) { return quantity<rep, unit>{value++}; }

		//! Pre-decrement.
		constexpr auto& operator --() {
			--value;
			return *this;
		}

		//! Post-decrement.
		constexpr auto operator --(int) { return quantity<rep, unit>{value--}; }

		//! The negation of this quantity.
		constexpr auto operator -() const { return quantity<rep, unit>{-value}; }

		//! Implicit conversion to the representation type, if the quantity is unitless.
		template <typename = std::enable_if_t<detail::is_same_unit_v<unit, unit_t<>>>>
		constexpr operator rep() const noexcept { return value; }
	};

	//! A unit-less quantity with representation @p Rep.
	template <typename Rep>
	using unitless = quantity<Rep, unit_t<>>;

	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator ==(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) == static_cast<common_rep>(q2.value);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator !=(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) != static_cast<common_rep>(q2.value);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator <(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) < static_cast<common_rep>(q2.value);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator <=(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) <= static_cast<common_rep>(q2.value);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator >(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) > static_cast<common_rep>(q2.value);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr bool operator >=(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.value) >= static_cast<common_rep>(q2.value);
	}

	//! Add quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator +(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to add quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit2, Unit2>, "Attempted to add quantities of different units.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.value) + static_cast<result_rep>(q2.value)};
	}

	//! Subtract quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator -(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to subtract quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit2, Unit2>, "Attempted to subtract quantities of different units.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.value) - static_cast<result_rep>(q2.value)};
	}

	//! Multiply a quantity by a scalar.
	template <typename Rep, typename Unit, typename K, typename = std::enable_if_t<std::is_convertible_v<K, Rep>>>
	constexpr auto operator *(quantity<Rep, Unit> const& q, K const& k) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.value)* static_cast<result_rep>(k)};
	}
	//! Multiply a scalar by a quantity.
	template <typename K, typename Rep, typename Unit, typename = std::enable_if_t<std::is_convertible_v<K, Rep>>>
	constexpr auto operator *(K const& k, quantity<Rep, Unit> const& q) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(k)* static_cast<result_rep>(q.value)};
	}

	//! Divide a quantity by a scalar.
	template <typename Rep, typename Unit, typename K, typename = std::enable_if_t<std::is_convertible_v<K, Rep>>>
	constexpr auto operator /(quantity<Rep, Unit> const& q, K const& k) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.value) / static_cast<result_rep>(k)};
	}
	//! Divide a scalar by a quantity.
	template <typename K, typename Rep, typename Unit, typename = std::enable_if_t<std::is_convertible_v<K, Rep>>>
	constexpr auto operator /(K const& k, quantity<Rep, Unit> const& q) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, inverse_t<Unit>>{static_cast<result_rep>(k) / static_cast<result_rep>(q.value)};
	}

	//! Mod a quantity by a scalar.
	template <typename Rep, typename Unit, typename K, typename = std::enable_if_t<std::is_convertible_v<K, Rep>>>
	constexpr auto operator %(quantity<Rep, Unit> const& q, K const& k) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.value) % static_cast<result_rep>(k)};
	}

	//! Mod quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator %(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to mod quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.value) % static_cast<result_rep>(q2.value)};
	}

	//! Multiply quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator *(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to multiply quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, product_t<Unit1, Unit2>>{static_cast<result_rep>(q1.value)* static_cast<result_rep>(q2.value)};
	}

	//! Divide quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator /(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to divide quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, quotient_t<Unit1, Unit2>>{static_cast<result_rep>(q1.value) / static_cast<result_rep>(q2.value)};
	}

	//! The square root of a quantity.
	//! @note The powers of the quantity's units must be divisible by two.
	//! @note Uses ADL to find a @p sqrt() function for the quantity's representation, defaulting to @p std::sqrt().
	template <typename Rep, typename Unit>
	constexpr auto sqrt(quantity<Rep, Unit> const& q) {
		using gcem::sqrt;
		return quantity<Rep, cancel::exponential_t<1, 2, Unit>>{static_cast<Rep>(sqrt(q.value))};
	}

	//! Floating-point mod a quantity by a scalar.
	template
		< typename Rep, typename Unit, typename K
		, typename = std::enable_if_t<std::is_floating_point_v<Rep> && std::is_convertible_v<K, Rep>>
		>
		constexpr auto fmod(quantity<Rep, Unit> const& q, K const& k) {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(std::fmodl(q.value, k))};
	}

	//! Floating-point mod quantities of compatible representation and the same unit.
	template
		< typename Rep1, typename Unit1, typename Rep2, typename Unit2
		, typename = std::enable_if_t<std::is_floating_point_v<std::common_type_t<Rep1, Rep2>>>
		>
		constexpr auto fmod(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to mod quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(fmodl(q1.value, q2.value))};
	}

	//! Cast a quantity to a quantity with the same unit but a different representation.
	template <typename ToQuantity, typename FromRep, typename FromUnit>
	constexpr auto quantity_cast(quantity<FromRep, FromUnit> const& q) {
		static_assert(detail::is_same_unit_v<FromUnit, typename ToQuantity::unit>, "Cannot cast quantity to a different unit.");
		return ToQuantity{static_cast<typename ToQuantity::rep>(q.value)};
	}

	namespace detail {
		//! Extend product to quantities.
		template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
		struct detail::product<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>> {
			static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Quantity types have incompatible representations.");
			using type = quantity<std::common_type_t<Rep1, Rep2>, product_t<Unit1, Unit2>>;
		};

		//! Extend inverse to quantities.
		template <typename Rep, typename Unit>
		struct detail::inverse<quantity<Rep, Unit>> {
			using type = quantity<Rep, detail::inverse_t<Unit>>;
		};

		//! Extend quotient to quantities.
		template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
		struct detail::quotient<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>> {
			static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Quantity types have incompatible representations.");
			using type = quantity<std::common_type_t<Rep1, Rep2>, quotient_t<Unit1, Unit2>>;
		};
	}
}
