//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "agents/action.hpp"
#include "quantities/game_time.hpp"
#include "utility/complete.hpp"

namespace ql {
	struct being;
	struct gatestone;

	namespace magic {
		//! Heals a being.
		struct heal {
			static constexpr char const* name = "Heal";
			static constexpr magic::color color = magic::color::white;
			static constexpr tick cooldown = 5_tick;
			static constexpr tick base_incant_time = 10_tick;

			complete cast(being& caster, gatestone& gatestone, action::cont cont);
		};
	}
}
