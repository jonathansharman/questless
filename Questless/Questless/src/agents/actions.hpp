//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Contains a set of standard actions beings can perform.

#pragma once

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	void turn(ent turner_id, region_tile::direction direction);
	void walk(ent walker_id, region_tile::direction direction);
	void fly(ent flyer_id);
	void drop(ent dropper_id, ent item_id);
	void toss(ent thrower_id, ent item_id);
}
