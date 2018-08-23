//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/quiver.hpp"

namespace ql {
	quiver::quiver(ql::inventory inventory, ql::id<item> id)
		: item{id}
		, inventory{std::move(inventory)}
	{}

	std::vector<uptr<action>> quiver::actions() {
		std::vector<uptr<action>> actions;
		if (equipped()) {
			actions.push_back(unequip::make(*this));
		} else {
			actions.push_back(equip::make(*this));
			actions.push_back(drop::make(*this));
			actions.push_back(toss::make(*this));
		}
		return actions;
	}
}
