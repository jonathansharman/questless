//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines a type list type and related operations.

#pragma once

#include <type_traits>

namespace questless::type_list
{
	//! The empty type list.
	struct Empty
	{
		using length_t = std::integral_constant<int, 0>;
	};

	//! Constructs a linked type list from @p HeadType and @p TailType.
	template <typename HeadType, typename TailType>
	struct Cons
	{
		using head_t = HeadType;
		using tail_t = TailType;

		using length_t = std::integral_constant<int, 1 + tail_t::length_t::value>;
	};

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	struct Of;

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	using of_t = typename Of<Types...>::type;

	//! Constructs an empty type list.
	template <>
	struct Of<>
	{
		using type = Empty;
	};

	//! Constructs a linked type list from @p HeadType and @p TailTypes.
	template <typename HeadType, typename... TailTypes>
	struct Of<HeadType, TailTypes...>
	{
		using type = Cons<HeadType, of_t<TailTypes...>>;
	};

	//! Applies @p Xform to each type in @p ListType.
	template <template <typename> typename Xform, typename... ListType>
	struct ApplyXform;

	//! Applies @p Xform to each type in @p ListType.
	template <template <typename> typename Xform, typename... ListType>
	using apply_xform_t = typename ApplyXform<Xform, ListType...>::type;

	//! Applies @p Xform to the empty type list, which results in the empty type list again.
	template <template <typename> typename Xform>
	struct ApplyXform<Xform, Empty>
	{
		using type = Empty;
	};

	//! Applies @p Xform to the linked type list with @p HeadType and @p TailType.
	template <template <typename> typename Xform, typename HeadType, typename TailType>
	struct ApplyXform<Xform, Cons<HeadType, TailType>>
	{
		using type = Cons<typename Xform<HeadType>::type, type_list::apply_xform_t<Xform, TailType>>;
	};

	//! Const-qualifies @p ListType.
	template <typename... ListType>
	using add_const_t = apply_xform_t<std::add_const, ListType...>;
}
