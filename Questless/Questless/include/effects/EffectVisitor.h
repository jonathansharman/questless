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

	using EffectConstVisitor = Visitor
		< EagleEyeEffect const
		, InjuryEffect const
		, LightningBoltEffect const
		>;
}
