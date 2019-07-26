//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct hud;

	//! Spawns a being into the world for the player.
	//! @param region_id The ID of the region to spawn the player into.
	//! @param hud A pointer to the HUD through which the player will interact with the world.
	//! @return The ID of the spawned player.
	ent spawn_player(ent region_id, hud* hud);
}
