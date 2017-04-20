//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <stdexcept>

namespace questless
{
	class EagleEyeEffect;
	class InjuryEffect;
	class LightningBoltEffect;

	//! Visitor type for effects.
	struct EffectVisitor
	{
		virtual ~EffectVisitor() = default;

		virtual void visit(EagleEyeEffect const&) = 0;
		virtual void visit(InjuryEffect const&) = 0;
		virtual void visit(LightningBoltEffect const&) = 0;
	};
}
