//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	class EagleEyeEffect;
	class InjuryEffect;
	class LightningBoltEffect;

	using EffectSubtypeList = type_list::of_t
		< EagleEyeEffect
		, InjuryEffect
		, LightningBoltEffect
		>;

	DEFINE_VISITORS(Effect, EffectSubtypeList)
}
