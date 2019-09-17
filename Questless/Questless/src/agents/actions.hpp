//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Contains a set of standard actions beings can perform.

#pragma once

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	auto turn(id turner_id, tile_hex::direction direction) -> void;

	auto walk(id walker_id, tile_hex::direction direction) -> void;

	//! Moves or turns towards the given direction, depending on the current heading and the value of @p strafe.
	auto move(id mover_id, tile_hex::direction direction, bool strafe) -> void;

	auto fly(id flyer_id) -> void;

	auto drop(id dropper_id, id item_id) -> void;

	auto toss(id thrower_id, id item_id) -> void;
}
