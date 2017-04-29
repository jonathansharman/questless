//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines generic Visitor and Element types, to implement the visitor pattern.

#pragma once

#include <tuple>

//! @todo This thing is complicated... Need more documentation, including examples.

namespace questless
{
	//! Visitor template declaration.
	template <typename... ElementTypes>
	class Visitor;

	//! Specialization of visitor template for the base case: zero types.
	template <>
	class Visitor<> {};

	//! Specialization of visitor template for the base case: one type.
	template <typename ElementType>
	class Visitor<ElementType>
	{
	public:
		//! Visits @p element.
		virtual void visit(ElementType& element) = 0;
	};

	//! Specialization of the visitor template for the recursive case: one or more types.
	template <typename FirstElementType, typename... RestElementTypes>
	class Visitor<FirstElementType, RestElementTypes...> : public Visitor<RestElementTypes...>
	{
	public:
		virtual ~Visitor() = default;

		//! Visits @p element.
		virtual void visit(FirstElementType& element) = 0;

		// Import definitions of visit functions from recursive base class.
		using Visitor<RestElementTypes...>::visit;
	};

	//! An element that accepts modifying visitors of type @p VisitorType.
	template <typename MutableVisitorType>
	class MutableElement
	{
	public:
		virtual void accept(MutableVisitorType& visitor) = 0;
	};

	//! An element that accepts non-modifying visitors of type @p VisitorType.
	template <typename ConstVisitorType>
	class ConstElement
	{
	public:
		virtual void accept(ConstVisitorType& visitor) const = 0;
	};

	//! An element that accepts both modifying and non-modifying visitors of type @p VisitorType.
	template <typename MutableVisitorType, typename ConstVisitorType>
	class Element
	{
	public:
		virtual void accept(MutableVisitorType& visitor) = 0;
		virtual void accept(ConstVisitorType& visitor) const = 0;
	};
}

//! Defines ParentType##MutableBase, implementing non-const ElementType::accept for ParentType using CRTP. Pulls the ParentClass constructors from protected to public.
#define DEFINE_MUTABLE_ELEMENT_BASE(ParentType, ElementType) \
template <typename Derived> \
class ParentType##MutableBase : public ParentType \
{ \
public: \
	void accept(ElementType##MutableVisitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
protected: \
	using ParentType::ParentType; \
};

//! Defines ParentType##ConstBase, implementing const ElementType::accept for ParentType using CRTP. Pulls the ParentClass constructors from protected to public.
#define DEFINE_CONST_ELEMENT_BASE(ParentType, ElementType) \
template <typename Derived> \
class ParentType##ConstBase : public ParentType \
{ \
public: \
	void accept(ElementType##ConstVisitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	using ParentType::ParentType; \
};

//! Defines ParentType##ConstBase, implementing const and non-cost ElementType::accept for ParentType using CRTP. Pulls the ParentClass constructors from protected to public.
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

//! Defines ParentType##MutableBase, implementing non-const ElementType::accept for ParentType using CRTP. Makes a trivial protected default constructor.
#define DEFINE_MUTABLE_ELEMENT_BASE_MAKE_CTOR(ParentType, ElementType) \
template <typename Derived> \
class ParentType##MutableBase : public ParentType \
{ \
public: \
	void accept(ElementType##MutableVisitor& visitor) final { visitor.visit(static_cast<Derived&>(*this)); } \
protected: \
	ParentType##MutableBase() {} \
};

//! Defines ParentType##ConstBase, implementing const ElementType::accept for ParentType using CRTP. Makes a trivial protected default constructor.
#define DEFINE_CONST_ELEMENT_BASE_MAKE_CTOR(ParentType, ElementType) \
template <typename Derived> \
class ParentType##ConstBase : public ParentType \
{ \
public: \
	void accept(ElementType##ConstVisitor& visitor) const final { visitor.visit(static_cast<Derived const&>(*this)); } \
protected: \
	ParentType##ConstBase() {} \
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
	ParentType##ConstBase() {} \
};
