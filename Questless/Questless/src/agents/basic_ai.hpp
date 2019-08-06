//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

#include <future>
#include <variant>

namespace ql {
	namespace effects {
		struct effect;
	}

	//! A basic AI, suitable for most NPCs.
	struct basic_ai {
		basic_ai(id id);

		std::future<void> act();

		void perceive(effects::effect const& effect);

	private:
		id _id;

		struct idle_state {};
		struct walk_state {};
		struct attack_state {
			ql::id target_id;
		};

		std::variant<idle_state, walk_state, attack_state> _state = idle_state{};
	};
}
