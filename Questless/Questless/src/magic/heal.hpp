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

		void cast(ent caster_id, ent gatestone_id, ent target_id, health healing);
	};
}
