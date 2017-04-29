//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

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

	using BodyPartMutableVisitor = Visitor<Head, Torso, Arm, Hand, Leg, Foot, Wing, Tail>;
}
