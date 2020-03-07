//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql::magic {
	//! Teleports the caster some distance from its current location.
	struct teleport {
		static constexpr magic::color color = magic::color::yellow;
		static constexpr tick cooldown = 15_tick;

		auto cast(reg& reg, id caster_id, id gatestone_id, tile_hex_point target) -> void;
	};
}
