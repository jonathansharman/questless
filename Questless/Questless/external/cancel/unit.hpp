//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Defines a type list type and related operations.

#pragma once

#include "list.hpp"
#include "algebra.hpp"

namespace cancel {
	namespace detail {
		//! A list of type tags, each representing a unit.
		template <typename... Tags>
		struct unit;

		template <typename FirstTag, typename... RestTags>
		struct unit<FirstTag, RestTags...> {
			using type = product_t
				< list_t<type_power<FirstTag, 1>>
				, typename unit<RestTags...>::type
				>;
		};

		template <>
		struct unit<> {
			using type = list_t<>;
		};

		//! Checks whether @p Unit1 is a subset of @p Unit2.
		template <typename Unit1, typename Unit2>
		struct is_subset_unit;

		//! Checks whether @p Unit1 is a subset of @p Unit2.
		template <typename Unit1, typename Unit2>
		constexpr bool is_subset_unit_v = is_subset_unit<Unit1, Unit2>::value;

		template <typename Head, typename Tail, typename Unit2>
		struct is_subset_unit<cons<Head, Tail>, Unit2> {
			static constexpr bool value = Head::power == get_power_v<typename Head::type, Unit2> && is_subset_unit_v<Tail, Unit2>;
		};

		template <typename Unit2>
		struct is_subset_unit<empty, Unit2> : std::true_type {};

		//! Checks whether @p Unit1 and @p Unit2 represent the same unit (i.e. are equal modulo reordering).
		template <typename Unit1, typename Unit2>
		struct is_same_unit {
			static constexpr bool value = is_subset_unit_v<Unit1, Unit2> && is_subset_unit_v<Unit2, Unit1>;
		};

		//! Checks whether @p Unit1 and @p Unit2 represent the same unit (i.e. are equal modulo reordering).
		template <typename Unit1, typename Unit2>
		constexpr bool is_same_unit_v = is_same_unit<Unit1, Unit2>::value;
	}

	//! A list of type tags, each representing a unit.
	template <typename... Tags>
	using unit_t = typename detail::unit<Tags...>::type;
}
