//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	class head;
	class torso;
	class arm;
	class hand;
	class leg;
	class foot;
	class wing;
	class tail;

	using body_part_subtype_list = type_list::of_t
		< head
		, torso
		, arm
		, hand
		, leg
		, foot
		, wing
		, tail
		>;

	DEFINE_VISITORS(body_part, body_part_subtype_list)
}
