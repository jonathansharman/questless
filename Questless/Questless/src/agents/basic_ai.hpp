//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"
#include "reg.hpp"

namespace ql {
	//! A basic AI, suitable for most NPCs.
	struct basic_ai : agent {
		basic_ai(ent id) : agent{id} {}

		action::result act() final;

		void perceive(effects::effect const& effect) final;

	private:
		struct idle_state {};
		struct walk_state {};
		struct attack_state {
			ent target_id;
		};

		std::variant<idle_state, walk_state, attack_state> _state = idle_state{};
	};
}
