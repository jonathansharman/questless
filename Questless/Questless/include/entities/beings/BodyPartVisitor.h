//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	class Head;
	class Torso;
	class Arm;
	class Hand;
	class Leg;
	class Foot;
	class Wing;
	class Tail;

	using BodyPartSubtypeList = type_list::of_t
		< Head
		, Torso
		, Arm
		, Hand
		, Leg
		, Foot
		, Wing
		, Tail
		>;

	DEFINE_VISITORS(BodyPart, BodyPartSubtypeList)
}
