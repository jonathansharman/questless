//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"

namespace ql::magic {
	//! Teleports the caster some distance from its current location.
	struct teleport {
		static constexpr magic::color color = magic::color::yellow;
		static constexpr tick cooldown = 15_tick;

		void cast(ent caster_id, ent gatestone_id, region_tile::point target);
	};
}
