//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/game_time.hpp"
#include "reg.hpp"

namespace ql {
	//! The remains of a deceased corporeal being.
	struct corpse {
		ent being_id;
	};

	//! Makes @p id a corpse of @p being_id.
	void make_corpse(ent id, ent being_id);
}
