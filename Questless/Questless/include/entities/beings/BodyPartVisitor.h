//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

namespace questless
{
	class Head;
	class Torso;
	class Arm;
	class Hand;
	class Leg;
	class Foot;
	class Wing;
	class Tail;

	//! Visitor type for body parts.
	class BodyPartVisitor
	{
	public:
		virtual ~BodyPartVisitor() = default;

		virtual void visit(Head&) = 0;
		virtual void visit(Torso&) = 0;
		virtual void visit(Arm&) = 0;
		virtual void visit(Hand&) = 0;
		virtual void visit(Leg&) = 0;
		virtual void visit(Foot&) = 0;
		virtual void visit(Wing&) = 0;
		virtual void visit(Tail&) = 0;
	};
}
