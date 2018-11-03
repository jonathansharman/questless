//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Types and type functions to manage algebraic powers of types.

#pragma once

#include "list.hpp"

namespace meta {
	namespace detail {
		//! A type representing the power (exponent) of a type.
		template <typename T, int Power>
		struct type_power {
			using type = T;
			static constexpr int power = Power;
		};

		//! The power of the given type in the given type power list.
		template <typename T, typename TypePowerList>
		struct get_power;

		//! The value of the power of the given type in the given type power list.
		template <typename T, typename TypePowerList>
		constexpr int get_power_v = get_power<T, TypePowerList>::value;

		template <typename T>
		struct get_power<T, empty> { static constexpr int value = 0; };

		template <typename T, typename Head, typename Tail>
		struct get_power<T, cons<Head, Tail>> {
			static constexpr int value = std::is_same_v<T, typename Head::type>
				? Head::power
				: get_power_v<T, Tail>
				;
		};

		//! The product of two type power lists.
		template <typename... TypePowerLists>
		struct product;

		//! The type of the product of two type power lists.
		template <typename... TypePowerLists>
		using product_t = typename product<TypePowerLists...>::type;

		template <typename Head, typename Tail, typename T, int Power>
		struct product<cons<Head, Tail>, type_power<T, Power>> {
			using type = std::conditional_t
				< std::is_same_v<T, typename Head::type>
				, std::conditional_t // Found matching type powers.
					< Head::power + Power == 0
					, Tail // Cancel.
					, cons<type_power<T, Head::power + Power>, Tail> // Add powers.
					>
				, cons<Head, product_t<Tail, type_power<T, Power>>> // Recurse.
				>;
		};

		template <typename T, int Power>
		struct product<empty, type_power<T, Power>> {
			using type = cons<type_power<T, Power>, empty>;
		};

		template <typename TypePowerList1, typename Head, typename Tail>
		struct product<TypePowerList1, cons<Head, Tail>> {
			using type = product_t<product_t<TypePowerList1, Head>, Tail>;
		};

		template <typename TypePowerList1>
		struct product<TypePowerList1, empty> {
			using type = TypePowerList1;
		};

		//! The inverse of the given type type power list.
		template <typename TypePowerList>
		struct inverse;

		//! The type of the inverse of the given type type power list.
		template <typename TypePowerList>
		using inverse_t = typename inverse<TypePowerList>::type;

		template <>
		struct inverse<empty> { using type = empty; };

		template <typename Head, typename Tail>
		struct inverse<cons<Head, Tail>> {
			using type = cons<type_power<typename Head::type, -Head::power>, inverse_t<Tail>>;
		};

		//! The quotient of two type power lists.
		template <typename TypePowerList1, typename TypePowerList2>
		struct quotient {
			using type = product_t<TypePowerList1, inverse_t<TypePowerList2>>;
		};

		//! The type of the quotient of two type power lists.
		template <typename TypePowerList1, typename TypePowerList2>
		using quotient_t = typename quotient<TypePowerList1, TypePowerList2>::type;

		//! A type power list to the power of a constant positive rational integral exponent.
		template <int ExponentNumerator, int ExponentDenominator, typename... TypePowerList>
		struct exponential;

		//! The type of a type power list to the power of a constant positive rational integral exponent.
		template <int ExponentNumerator, int ExponentDenominator, typename... TypePowerList>
		using exponential_t = typename exponential<ExponentNumerator, ExponentDenominator, TypePowerList...>::type;

		template <int ExponentNumerator, int ExponentDenominator, typename Head, typename Tail>
		struct exponential<ExponentNumerator, ExponentDenominator, cons<Head, Tail>> {
			static_assert(ExponentDenominator != 0, "Exponent denominator cannot be zero.");
			static_assert(ExponentNumerator == 0 || (ExponentNumerator < 0 == ExponentDenominator < 0), "Exponent must be nonnegative.");
			static_assert((Head::power * ExponentNumerator) % Exponent == 0, "A type power exponential must result in integral powers.");

			using type = cons
				< type_power<typename Head::T, Head::power * ExponentNumerator / ExponentDenominator>
				, exponential_t<ExponentNumerator, ExponentDenominator, Tail>
				>;
		};

		template <int ExponentNumerator, int ExponentDenominator>
		struct exponential<ExponentNumerator, ExponentDenominator, empty> {
			using type = empty;
		};
	}

	//! The value of the power of the given type in the given type power list.
	template <typename T, typename TypePowerList>
	constexpr int get_power_v = detail::get_power<T, TypePowerList>::value;

	// The type of the product of two type power lists.
	template <typename... TypePowerLists>
	using product_t = typename detail::product<TypePowerLists...>::type;

	//! The type of the inverse of the given type type power list.
	template <typename TypePowerList>
	using inverse_t = typename detail::inverse<TypePowerList>::type;

	// The type of the quotient of two type power lists.
	template <typename TypePowerList1, typename TypePowerList2>
	using quotient_t = typename detail::quotient<TypePowerList1, TypePowerList2>::type;

	//! The type of a type power list to the power of a constant positive rational integral exponent.
	template <int ExponentNumerator, int ExponentDenominator, typename... TypePowerList>
	using exponential_t = typename detail::exponential<ExponentNumerator, ExponentDenominator, TypePowerList...>::type;
}
