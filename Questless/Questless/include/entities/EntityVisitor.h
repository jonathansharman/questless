/**
* @file    EntityVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Entity visitor abstract base class.
*/

#pragma once

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

		virtual void visit(Human const&) = 0;
		virtual void visit(Goblin const&) = 0;

		virtual void visit(Corpse const&) = 0;
		virtual void visit(ItemBox const&) = 0;
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

		virtual void visit(Human const&) override { throw BadEntityVisit{_message}; }
		virtual void visit(Goblin const&) override { throw BadEntityVisit{_message}; }

		virtual void visit(Corpse const&) override { throw BadEntityVisit{_message}; }
		virtual void visit(ItemBox const&) override { throw BadEntityVisit{_message}; }
	private:
		std::string _message;
	};
}
