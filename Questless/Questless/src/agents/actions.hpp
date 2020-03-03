//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Contains a set of standard actions beings can perform.

#pragma once

#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	auto turn(reg& reg, id turner_id, tile_hex::direction direction) -> void;

	auto walk(reg& reg, id walker_id, tile_hex::direction direction) -> void;

	//! Moves or turns towards the given direction, depending on the current heading and the value of @p strafe.
	auto move(reg& reg, id mover_id, tile_hex::direction direction, bool strafe) -> void;

	auto fly(reg& reg, id flyer_id) -> void;

	auto drop(reg& reg, id dropper_id, id item_id) -> void;

	auto toss(reg& reg, id thrower_id, id item_id) -> void;
}
