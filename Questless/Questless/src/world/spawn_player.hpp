//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct hud;

	//! Creates a being for the player and spawns it into the world.
	//! @param player_id The ID of an entity that has been created but hasn't been assigned components yet. Will be
	//! initialized as a player character.
	//! @param region_id The ID of an already-initialized region.
	//! @return The ID of the spawned player.
	auto create_and_spawn_player(id player_id, id region_id, hud* hud) -> void;
}
