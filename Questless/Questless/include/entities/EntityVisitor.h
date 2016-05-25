/**
* @file    EntityVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Entity visitor abstract base class.
*/

#ifndef ENTITY_VISITOR_H
#define ENTITY_VISITOR_H

#include <string>
using std::string;
#include <exception>
using std::runtime_error;

namespace questless
{
	class Human;
	class Goblin;

	class EntityVisitor
	{
	public:
		virtual ~EntityVisitor() = default;

		virtual void visit(const Human&) = 0;
		virtual void visit(const Goblin&) = 0;
	};

	class BadEntityVisit : public runtime_error
	{
	public:
		BadEntityVisit(string message) : runtime_error{move(message)} {}
	};

	class TrivialEntityVisitor : public EntityVisitor
	{
	public:
		TrivialEntityVisitor(string message) : _message{move(message)} {}

		virtual ~TrivialEntityVisitor() = default;

		virtual void visit(const Human&) override { throw BadEntityVisit{_message}; }
		virtual void visit(const Goblin&) override { throw BadEntityVisit{_message}; }
	private:
		string _message;
	};
}

#endif