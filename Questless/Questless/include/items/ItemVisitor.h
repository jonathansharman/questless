/**
* @file    ItemVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the Item class.
*/

#ifndef ITEM_VISITOR_H
#define ITEM_VISITOR_H

#include <string>
#include <exception>
#include <memory>

namespace questless
{
	class Scroll;
	class Quarterstaff;

	class ItemVisitor
	{
	public:
		virtual ~ItemVisitor() = default;

		virtual void visit(const Scroll&) = 0;
		virtual void visit(const Quarterstaff&) = 0;
	};

	class BadItemVisit : public std::runtime_error
	{
	public:
		BadItemVisit(std::string message) : std::runtime_error{std::move(message)} {}
	};

	class TrivialItemVisitor : public ItemVisitor
	{
	public:
		TrivialItemVisitor(std::string message) : _message{std::move(message)} {}

		virtual ~TrivialItemVisitor() = default;

		virtual void visit(const Scroll&) override { throw BadItemVisit{_message}; }
		virtual void visit(const Quarterstaff&) override { throw BadItemVisit{_message}; }
	private:
		std::string _message;
	};
}

#endif
