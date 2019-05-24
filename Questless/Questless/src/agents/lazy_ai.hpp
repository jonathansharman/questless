//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

namespace ql {
	//! An AI that does nothing.
	struct lazy_ai : agent {
		lazy_ai(ent id) : agent{id} {}

		complete act() final {
			return complete{};
		}

		void perceive(effects::effect const&) final {}
	};
}
