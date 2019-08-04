//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/dynamic_nonnegative.hpp"
#include "magic/color.hpp"

#include "quantities/misc.hpp"
#include "reg.hpp"

#include <string>

namespace ql {
	//! A gem that can hold spell charges.
	struct gatestone {
		id id;

		//! The color of spell this gatestone can be used to cast.
		magic::color color;

		//! The amount of mana this gatestone holds.
		dynamic_nonnegative<mana> charge;

		//! Time left before this gatestone can be used again.
		dynamic_nonnegative<tick> cooldown;

		void update(tick elapsed);
	};

	id make_gatestone(id id, magic::color color, dynamic_nonnegative<mana> charge, dynamic_nonnegative<tick> cooldown);
}
