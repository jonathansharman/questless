//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines generic visitor type, generic element type, and other tools to implement the visitor pattern.

//! @todo Add more documentation, including examples.

#pragma once

#include "type_list.h"

namespace questless
{
	//! Visitor template for at least one type (recursive case).
	template <typename ElementSubtypeList, int length>
	class Visitor : public Visitor<typename ElementSubtypeList::tail_t, ElementSubtypeList::tail_t::length_t::value>
	{
	public:
		virtual ~Visitor() = default;

		//! Visits @p element.
		virtual void visit(typename ElementSubtypeList::head_t& element) = 0;

		// Import definitions of visit functions from recursive base class.
		using Visitor<typename ElementSubtypeList::tail_t, ElementSubtypeList::tail_t::length_t::value>::visit;
	};

	//! Specialization of visitor template for zero types.
	template <>
	class Visitor<type_list::Empty, 0> {};

	//! Specialization of visitor template for one type.
	template <typename ElementSubtypeList>
	class Visitor<ElementSubtypeList, 1>
	{
	public:
		//! Visits @p element.
		virtual void visit(typename ElementSubtypeList::head_t& element) = 0;
	};

	//! An element that accepts both modifying visitors of type @p MutableVisitorType and non-modifying visitors of type @p ConstVisitorType.
	template <typename ElementSubtypeList>
	class Element
	{
	public:
		virtual void accept(Visitor<ElementSubtypeList, ElementSubtypeList::length_t::value>& visitor) = 0;
		virtual void accept(Visitor<type_list::add_const_t<ElementSubtypeList>, ElementSubtypeList::length_t::value>& visitor) const = 0;
	};
}

//! Defines ElementType##MutableVisitor and ElementType##ConstVisitor.
#define DEFINE_VISITORS(ElementType, ElementSubtypeList) \
using ElementType##MutableVisitor = Visitor<ElementSubtypeList, ElementSubtypeList::length_t::value>; \
using ElementType##ConstVisitor = Visitor<type_list::add_const_t<ElementSubtypeList>, ElementSubtypeList::length_t::value>;

//! Defines ParentType##ConstBase, implementing const and non-cost ElementType::accept for ParentType using CRTP. Pulls in the visible ParentClass constructors as protected.
#define DEFINE_ELEMENT_BASE(ParentType, ElementType) \
template <typename Derived> \
class ParentType##Base : public ParentType \
{ \
public: \
	void accept(ElementType##MutableVisitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
	void accept(ElementType##ConstVisitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	using ParentType::ParentType; \
};

//! Defines ParentType##ConstBase, implementing const and non-const ElementType::accept for ParentType using CRTP. Makes a trivial protected default constructor.
#define DEFINE_ELEMENT_BASE_MAKE_CTOR(ParentType, ElementType) \
template <typename Derived> \
class ParentType##Base : public ParentType \
{ \
public: \
	void accept(ElementType##MutableVisitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
	void accept(ElementType##ConstVisitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	ParentType##Base() {} \
};
