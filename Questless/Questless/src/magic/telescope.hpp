//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"
#include "spell.hpp"

#include "agents/action.hpp"

namespace ql::magic {
	//! Temporarily increases the caster's visual acuity.
	struct telescope {
		static constexpr magic::color color = magic::color::green;
		static constexpr tick cooldown = 5_tick;

		struct cast : action {
			ent gatestone_id;

			result perform(being& caster) final;
		};
	};
}
