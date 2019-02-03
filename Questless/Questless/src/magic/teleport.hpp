//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"

namespace ql {
	struct gatestone;

	namespace magic {
		//! Teleports the caster some distance from its current location.
		struct teleport {
			static constexpr char const* name = "Teleport";
			static constexpr magic::color color = magic::color::yellow;
			static constexpr tick cooldown = 15_tick;
			static constexpr tick base_incant_time = 20_tick;

			complete cast(being& caster, gatestone& gatestone, action::cont cont);

		private:
			static constexpr auto _cost_factor = 5.0_mp / 1_span;
			static constexpr span _range = 15_span;
		};
	}
}
