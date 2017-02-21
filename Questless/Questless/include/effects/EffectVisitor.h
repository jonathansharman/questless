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

	struct EffectVisitor
	{
		virtual ~EffectVisitor() = default;

		virtual void visit(LightningBoltEffect const&) = 0;
		virtual void visit(InjuryEffect const&) = 0;
	};
}
