//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines a type list type and related operations.

#pragma once

#include <type_traits>

namespace ql::type_list
{
	//! The empty type list.
	struct empty
	{
		using length_type = std::integral_constant<int, 0>;
	};

	//! Constructs a linked type list from @p HeadType and @p TailType.
	template <typename HeadType, typename TailType>
	struct cons
	{
		using head_type = HeadType;
		using tail_type = TailType;

		using length_type = std::integral_constant<int, 1 + tail_type::length_type::value>;
	};

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	struct of;

	//! Constructs a type list from the given variadic list of types.
	template <typename... Types>
	using of_t = typename of<Types...>::type;

	//! Constructs an empty type list.
	template <>
	struct of<>
	{
		using type = empty;
	};

	//! Constructs a linked type list from @p HeadType and @p TailTypes.
	template <typename HeadType, typename... TailTypes>
	struct of<HeadType, TailTypes...>
	{
		using type = cons<HeadType, of_t<TailTypes...>>;
	};

	//! Applies @p Xform to each type in @p ListType.
	template <template <typename> typename Xform, typename... ListType>
	struct apply_xform;

	//! Applies @p Xform to each type in @p ListType.
	template <template <typename> typename Xform, typename... ListType>
	using apply_xform_t = typename apply_xform<Xform, ListType...>::type;

	//! Applies @p Xform to the empty type list, which results in the empty type list again.
	template <template <typename> typename Xform>
	struct apply_xform<Xform, empty>
	{
		using type = empty;
	};

	//! Applies @p Xform to the linked type list with @p HeadType and @p TailType.
	template <template <typename> typename Xform, typename HeadType, typename TailType>
	struct apply_xform<Xform, cons<HeadType, TailType>>
	{
		using type = cons<typename Xform<HeadType>::type, type_list::apply_xform_t<Xform, TailType>>;
	};

	//! Const-qualifies @p ListType.
	template <typename... ListType>
	using add_const_t = apply_xform_t<std::add_const, ListType...>;
}
