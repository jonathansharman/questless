//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines generic visitor type, generic element type, and other tools to implement the visitor pattern.

//! @todo Add more documentation, including examples.

#pragma once

#include "type_list.h"

namespace ql
{
	//! Visitor template for at least one type (recursive case).
	template <typename ElementSubtypeList, int Length = ElementSubtypeList::length_type::value>
	class visitor : public visitor<typename ElementSubtypeList::tail_type, ElementSubtypeList::tail_type::length_type::value>
	{
	public:
		virtual ~visitor() = default;

		//! Visits @p element.
		virtual void visit(typename ElementSubtypeList::head_type& element) = 0;

		// Import definitions of visit functions from recursive base class.
		using visitor<typename ElementSubtypeList::tail_type, ElementSubtypeList::tail_type::length_type::value>::visit;
	};

	//! Specialization of visitor template for zero types.
	template <>
	class visitor<type_list::empty, 0> {};

	//! Specialization of visitor template for one type.
	template <typename ElementSubtypeList>
	class visitor<ElementSubtypeList, 1>
	{
	public:
		//! Visits @p element.
		virtual void visit(typename ElementSubtypeList::head_type& element) = 0;
	};

	//! An element that accepts both modifying visitors of type @p MutableVisitorType and non-modifying visitors of type @p ConstVisitorType.
	template <typename ElementSubtypeList>
	class element
	{
	public:
		virtual void accept(visitor<ElementSubtypeList>& visitor) = 0;
		virtual void accept(visitor<type_list::add_const_t<ElementSubtypeList>>& visitor) const = 0;
	};
}

//! Defines ElementType##MutableVisitor and ElementType##ConstVisitor.
#define DEFINE_VISITORS(ElementType, ElementSubtypeList) \
using ElementType##_mutable_visitor = visitor<ElementSubtypeList>; \
using ElementType##_const_visitor = visitor<type_list::add_const_t<ElementSubtypeList>>;

//! Defines ParentType##ConstBase, implementing const and non-cost ElementType::accept for ParentType using CRTP. Pulls in the visible ParentClass constructors as protected.
#define DEFINE_ELEMENT_BASE(ParentType, ElementType) \
template <typename Derived> \
class ParentType##_base : public ParentType \
{ \
public: \
	void accept(ElementType##_mutable_visitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
	void accept(ElementType##_const_visitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	using ParentType::ParentType; \
};

//! Defines ParentType##ConstBase, implementing const and non-const ElementType::accept for ParentType using CRTP. Makes a trivial protected default constructor.
#define DEFINE_ELEMENT_BASE_MAKE_CTOR(ParentType, ElementType) \
template <typename Derived> \
class ParentType##_base : public ParentType \
{ \
public: \
	void accept(ElementType##_mutable_visitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
	void accept(ElementType##_const_visitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	ParentType##_base() {} \
};
