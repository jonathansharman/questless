//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player.hpp"

#include "ui/hud.hpp"

namespace ql {
	player::player(hud* hud) : _hud{hud} {}

	auto player::act() -> std::future<void> {
		// Allow the player to perform actions via the HUD until passing the turn.
		return _hud->pass_future();
	}

	auto player::perceive(effects::effect const& effect) -> void {
		_hud->render_effect(effect);
	}

	auto player::set_hud(hud& hud) -> void {
		_hud = &hud;
	}
}
