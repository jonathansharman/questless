//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

namespace ql {
	class arrow : public item_base<arrow> {
	public:
		arrow(ql::id<item> id = ql::id<item>::make()) : item_base<arrow>{id} {}

		std::string name() const final { return "Arrow"; }

		double weight() const final { return 0.1; }

		std::vector<uptr<action>> actions() final {
			std::vector<uptr<action>> actions;
			actions.push_back(drop::make(*this));
			actions.push_back(toss::make(*this));
			return actions;
		}
	};
}
