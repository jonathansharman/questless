//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless::spell
{
	class EagleEye;
	class Heal;
	class Shock;
	class Teleport;

	using SpellSubtypeList = type_list::of_t
		< EagleEye
		, Heal
		, Shock
		, Teleport
		>;

	DEFINE_VISITORS(Spell, SpellSubtypeList)
}
