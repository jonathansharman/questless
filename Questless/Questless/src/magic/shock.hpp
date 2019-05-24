//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "agents/action.hpp"
#include "damage/damage.hpp"
#include "quantities/game_time.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql::magic {
	//! Discharges a bolt of electricity to strike a tile.
	struct shock {
		static constexpr magic::color color = magic::color::red;
		static constexpr tick cooldown = 5_tick;

		struct cast : action {
			ent gatestone_id;
			region_tile::point target;
			dmg::shock damage;

			result perform(being& caster) final;
		};
	};
}
