//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

namespace ql::magic {
	//! Heals a being.
	struct heal {
		static constexpr magic::color color = magic::color::white;
		static constexpr tick cooldown = 5_tick;

		void cast(id caster_id, id gatestone_id, id target_id, health healing);
	};
}
