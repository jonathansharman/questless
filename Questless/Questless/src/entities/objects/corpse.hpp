//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/game_time.hpp"

namespace ql {
	struct being;

	//! The remains of a deceased corporeal being.
	struct corpse {
		ql::id<being> being_id;

		double transparency() const {
			return 0.5;
		}

		bool blocks_movement() const {
			return true;
		}

		void update(tick /*elapsed*/) {}
	};
}
