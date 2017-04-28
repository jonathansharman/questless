//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines generic Visitor and Element types, to implement the visitor pattern.

#pragma once

#include <tuple>

namespace questless
{
	//! Visitor template declaration.
	template<typename... ElementTypes>
	class Visitor;

	//! Specialization of visitor template for the base case: one type.
	template<typename ElementType>
	class Visitor<ElementType>
	{
	public:
		//! Visits @p element.
		virtual void visit(ElementType& element) = 0;
	};

	//! Specialization of the visitor template for the recursive case: one or more types.
	template<typename FirstElementType, typename... RestElementTypes>
	class Visitor<FirstElementType, RestElementTypes...> : public Visitor<RestElementTypes...>
	{
	public:
		virtual ~Visitor() = default;

		//! Visits @p element.
		virtual void visit(FirstElementType& element) = 0;

		// Import definitions of visit functions from recursive base class.
		using Visitor<RestElementTypes...>::visit;
	};

	//! An element that accepts visitors of type @p VisitorType.
	template<typename DerivedType, typename VisitorType>
	class Element
	{
	public:
		virtual void accept(VisitorType& visitor)
		{
			visitor.visit(static_cast<DerivedType&>(*this));
		}
	};
}
