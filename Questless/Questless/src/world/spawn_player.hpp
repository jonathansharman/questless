//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct hud;

	//! Creates a being for the player and spawns it into the world.
	//! @param region_id The ID of the region to spawn the player into.
	//! @return The ID of the spawned player.
	auto create_and_spawn_player(id region_id) -> id;
}
