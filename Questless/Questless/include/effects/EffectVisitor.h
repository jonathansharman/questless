/**
* @file    EffectVisitor.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements the visitor pattern for the Effect class.
*/

#pragma once

#include <string>
#include <stdexcept>

namespace questless
{
	class LightningBoltEffect;
	class InjuryEffect;

	class EffectVisitor
	{
	public:
		virtual ~EffectVisitor() = default;

		virtual void visit(LightningBoltEffect const&) = 0;
		virtual void visit(InjuryEffect const&) = 0;
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

		virtual void visit(LightningBoltEffect const&) override { throw BadEffectVisit{_message}; }
		virtual void visit(InjuryEffect const&) override { throw BadEffectVisit{_message}; }
	private:
		std::string _message;
	};
}
