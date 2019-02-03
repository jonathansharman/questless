//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"
#include "spell.hpp"

#include "agents/action.hpp"

namespace ql {
	struct gatestone;

	namespace magic {
		//! Temporarily increases the caster's visual acuity.
		struct telescope {
			static constexpr char const* name = "Telescope";
			static constexpr magic::color color = magic::color::green;
			tick cooldown = 50_tick;
			tick base_incant_time = 10_tick;

			complete cast(being& caster, gatestone& gatestone, action::cont cont);

		private:
			static constexpr mana _cost = 10.0_mp;
		};
	}
}
