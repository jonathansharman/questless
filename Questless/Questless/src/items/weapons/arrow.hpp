//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

namespace ql {
	struct arrow : item_base<arrow> {
		arrow(ql::id<item> id = ql::id<item>::make()) : item_base<arrow>{id} {}

		std::string name() const final {
			return "Arrow";
		}

		ql::mass mass() const final {
			return 0.1_mass;
		}

		std::vector<uptr<action>> actions() final {
			std::vector<uptr<action>> actions;
			actions.push_back(drop::make(*this));
			actions.push_back(toss::make(*this));
			return actions;
		}
	};
}
