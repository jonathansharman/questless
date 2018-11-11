//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines a type list type and related operations.

#pragma once

#include <type_traits>

namespace cancel {
	//! The empty type list.
	struct empty {
		using length = std::integral_constant<int, 0>;
	};

	//! Constructs a linked type list from @p Head and @p Tail.
	template <typename Head, typename Tail>
	struct cons {
		using head = Head;
		using tail = Tail;

		using length = std::integral_constant<int, 1 + tail::length::value>;
	};

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	struct list;

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	using list_t = typename list<Types...>::type;

	//! Constructs an empty type list.
	template <>
	struct list<> {
		using type = empty;
	};

	//! Constructs a linked type list from @p Head and @p TailTypes.
	template <typename Head, typename... TailTypes>
	struct list<Head, TailTypes...> {
		using type = cons<Head, list_t<TailTypes...>>;
	};

	//////////////
	// Contains //
	//////////////

	//! Checks for containment of the type @p T in the type list @p TypeList.
	template <typename T, typename... TypeList>
	struct contains;

	//! Checks for containment of the type @p T in the type list @p TypeList.
	template <typename T, typename... TypeList>
	constexpr bool contains_v = contains<T, TypeList...>::value;

	template <typename T>
	struct contains<T, empty> : std::false_type {};

	template <typename T, typename Tail>
	struct contains<T, cons<T, Tail>> : std::true_type {};

	template <typename T, typename Head, typename Tail>
	struct contains<T, cons<Head, Tail>> : contains<T, Tail> {};

	////////////
	// Remove //
	////////////

	//! Removes the first occurrence of the type @p T in the type list @p TypeList.
	template <typename T, typename... TypeList>
	struct remove;

	//! Removes the first occurrence of the type @p T in the type list @p TypeList.
	template <typename T, typename... TypeList>
	using remove_t = typename remove<T, TypeList...>::type;

	template <typename T>
	struct remove<T, empty> {
		using type = empty;
	};

	template <typename T, typename Head, typename Tail>
	struct remove<T, cons<Head, Tail>> {
		using type = std::conditional
			< std::is_same_v<T, Head>
			, Tail
			, cons<Head, remove_t<Tail>>
			>;
	};

	///////////////
	// Transform //
	///////////////

	//! Applies @p Xform to each type in @p TypeList.
	template <template <typename> typename Xform, typename... TypeList>
	struct apply_xform;

	//! Applies @p Xform to each type in @p TypeList.
	template <template <typename> typename Xform, typename... TypeList>
	using apply_xform_t = typename apply_xform<Xform, TypeList...>::type;

	template <template <typename> typename Xform>
	struct apply_xform<Xform, empty> {
		using type = empty;
	};

	template <template <typename> typename Xform, typename Head, typename Tail>
	struct apply_xform<Xform, cons<Head, Tail>> {
		using type = cons<typename Xform<Head>::type, apply_xform_t<Xform, Tail>>;
	};

	//! Const-qualifies @p TypeList.
	template <typename... TypeList>
	using add_const_t = apply_xform_t<std::add_const, TypeList...>;
}
