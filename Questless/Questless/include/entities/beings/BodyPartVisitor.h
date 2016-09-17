/**
* @file    BodyPartVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the BodyPart class.
*/

#ifndef BODY_PART_VISITOR_H
#define BODY_PART_VISITOR_H

#include <string>
#include <exception>
#include <memory>

namespace questless
{
	class Torso;
	class Head;
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

		virtual void visit(Torso&) = 0;
		virtual void visit(Head&) = 0;
		virtual void visit(Arm&) = 0;
		virtual void visit(Hand&) = 0;
		virtual void visit(Leg&) = 0;
		virtual void visit(Foot&) = 0;
		virtual void visit(Wing&) = 0;
		virtual void visit(Tail&) = 0;
	};

	class BadBodyPartVisit : public std::runtime_error
	{
	public:
		BadBodyPartVisit(std::string message) : std::runtime_error{std::move(message)} {}
	};

	class TrivialBodyPartVisitor : public BodyPartVisitor
	{
	public:
		TrivialBodyPartVisitor(std::string message) : _message{std::move(message)} {}

		virtual ~TrivialBodyPartVisitor() = default;

		virtual void visit(Torso&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Head&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Arm&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Hand&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Leg&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Foot&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Wing&) override { throw BadBodyPartVisit{_message}; }
		virtual void visit(Tail&) override { throw BadBodyPartVisit{_message}; }
	private:
		std::string _message;
	};
}

#endif
