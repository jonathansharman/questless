//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player.hpp"

#include "ui/hud.hpp"

namespace ql {
	std::future<void> player::act() {
		// Allow the player to perform actions via the HUD until passing the turn.
		return hud->pass_future();
	}

	void player::perceive(effects::effect const& effect) {
		hud->render_effect(effect);
	}
}
