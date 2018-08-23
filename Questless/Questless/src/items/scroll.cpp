//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/scroll.hpp"
#include "entities/beings/being.hpp"

namespace ql {
	std::vector<uptr<action>> scroll::actions() {
		std::vector<uptr<action>> actions;
		if (blank()) {
			//! @todo Write?
		}
		actions.push_back(drop::make(*this));
		actions.push_back(toss::make(*this));
		return actions;
	}
}
