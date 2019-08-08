//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

#include <future>

namespace ql {
	namespace effects {
		struct effect;
	}
	struct hud;

	//! The agent representing the player's control over his or her character.
	struct player {
		player(hud* hud = nullptr);

		std::future<void> act();

		auto perceive(effects::effect const& effect) -> void;

		auto set_hud(hud& hud) -> void;

	private:
		hud* _hud;
	};
}
