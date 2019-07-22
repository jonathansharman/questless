//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effects/effect.hpp"
#include "ui/dialog/dialog.hpp"

namespace ql {
	namespace effects {
		struct effect;
	}
	struct hud;

	//! The agent representing the player's control over his or her character.
	struct player {
		ent id;

		hud* hud = nullptr;

		std::future<void> act();

		void perceive(effects::effect const& effect);
	};
}
