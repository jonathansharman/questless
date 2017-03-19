/**
* @file    BodyPartVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the BodyPart class.
*/

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
