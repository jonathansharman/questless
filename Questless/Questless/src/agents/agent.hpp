//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "basic_ai.hpp"
#include "lazy_ai.hpp"
#include "player.hpp"

#include "utility/visitation.hpp"

#include <variant>

namespace ql {
	//! Facilitates control of a being. Includes AIs and the player.
	struct agent {
		std::variant<basic_ai, lazy_ai, player> value;

		//! Allows the agent to perform actions.
		std::future<void> act() {
			return match(value, [](auto& value) { return value.act(); });
		}

		//! Causes the agent to perceive @p effect, possibly updating its state accordingly.
		virtual void perceive(effects::effect const& effect) {
			match(value, [&](auto& value) { value.perceive(effect); });
		}
	};
}
