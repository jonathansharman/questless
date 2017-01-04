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
#include <stdexcept>

namespace questless
{
	class Human;
	class Goblin;

	class Corpse;
	class ItemBox;

	class EntityVisitor
	{
	public:
		virtual ~EntityVisitor() = default;

		virtual void visit(const Human&) = 0;
		virtual void visit(const Goblin&) = 0;

		virtual void visit(const Corpse&) = 0;
		virtual void visit(const ItemBox&) = 0;
	};

	class BadEntityVisit : public std::runtime_error
	{
	public:
		BadEntityVisit(std::string message) : std::runtime_error{move(message)} {}
	};

	class TrivialEntityVisitor : public EntityVisitor
	{
	public:
		TrivialEntityVisitor(std::string message) : _message{move(message)} {}

		virtual ~TrivialEntityVisitor() = default;

		virtual void visit(const Human&) override { throw BadEntityVisit{_message}; }
		virtual void visit(const Goblin&) override { throw BadEntityVisit{_message}; }

		virtual void visit(const Corpse&) override { throw BadEntityVisit{_message}; }
		virtual void visit(const ItemBox&) override { throw BadEntityVisit{_message}; }
	private:
		std::string _message;
	};
}

#endif