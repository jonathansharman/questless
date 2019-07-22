//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/armor.hpp"

namespace ql::stats {
	//! The stats of a being as a whole.
	struct body {
		aggregate a{};
		dmg::armor armor{};
	};
}
