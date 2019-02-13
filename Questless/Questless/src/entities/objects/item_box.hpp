//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/game_time.hpp"
#include "utility/id.hpp"

#include <vector>

namespace ql {
	struct item;

	//! A box capable of holding some items.
	struct item_box {
		std::vector<ql::id<item>> items;

		double transparency() const {
			return 0.5;
		}

		bool blocks_movement() const {
			return false;
		}

		void update(tick /*elapsed*/) {}
	};
}
