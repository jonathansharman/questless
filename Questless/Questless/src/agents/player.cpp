//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player.hpp"

#include "ui/hud.hpp"

namespace ql {
	auto player::set_hud(hud* hud) -> void {
		_hud = hud;
	}

	std::future<void> player::act() {
		// Allow the player to perform actions via the HUD until passing the turn.
		return _hud->pass_future();
	}

	void player::perceive(effects::effect const& effect) {
		_hud->render_effect(effect);
	}
}
