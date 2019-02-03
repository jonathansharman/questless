//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"

namespace ql {
	struct gatestone;

	namespace magic {
		//! Discharges a bolt of electricity to strike a tile.
		struct shock {
			static constexpr char const* name = "Lightning Bolt";
			static constexpr magic::color color = magic::color::red;
			static constexpr tick cooldown = 5_tick;
			static constexpr tick base_incant_time = 15_tick;

			complete cast(being& caster, gatestone& gatestone, action::cont cont);
		};
	}
}
