//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/game_time.hpp"

namespace ql {
	//! A simple source of heat and light.
	struct campfire {
		double transparency() const {
			return 1.0;
		}

		bool blocks_movement() const {
			return true;
		}

		void update(tick /*elapsed*/) {}
	};
}
