/**
* @file    EffectVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the Effect class.
*/

#ifndef EFFECT_VISITOR_H
#define EFFECT_VISITOR_H

#include <string>
#include <stdexcept>

namespace questless
{
	class LightningBoltEffect;

	class EffectVisitor
	{
	public:
		virtual ~EffectVisitor() = default;

		virtual void visit(const LightningBoltEffect&) = 0;
	};

	class BadEffectVisit : public std::runtime_error
	{
	public:
		BadEffectVisit(std::string message) : std::runtime_error{std::move(message)} {}
	};

	class TrivialEffectVisitor : public EffectVisitor
	{
	public:
		TrivialEffectVisitor(std::string message) : _message{std::move(message)} {}

		virtual ~TrivialEffectVisitor() = default;

		virtual void visit(const LightningBoltEffect&) override { throw BadEffectVisit{_message}; }
	private:
		std::string _message;
	};
}

#endif
