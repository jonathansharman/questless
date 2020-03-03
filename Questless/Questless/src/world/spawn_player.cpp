//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "spawn_player.hpp"

#include "coordinates.hpp"
#include "region.hpp"

#include "agents/agent.hpp"
#include "agents/player.hpp"
#include "entities/beings/human.hpp"
#include "items/magic/gatestone.hpp"
#include "items/magic/scroll.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/spell.hpp"
#include "rsrc/fonts.hpp"

namespace ql {
	auto create_and_spawn_player(reg& reg, id region_id) -> id {
		// Get a valid spawn location from the region.
		location const location = reg.get<region>(region_id).get_spawn_location();

		// Create player being.
		id const player_id = reg.create();
		make_human(reg, player_id, location, {player{}});

		// Add to region.
		bool const success = reg.get<region>(region_id).try_add(player_id, location.coords);
		assert(success);

		// Fill inventory with starting items.
		auto& player_inv = reg.get<inventory>(player_id);

		// Give the player some magic scrolls.
		player_inv.add(make_scroll(reg, reg.create(), {{magic::shock{}}}));
		player_inv.add(make_scroll(reg, reg.create(), {{magic::heal{}}}));
		player_inv.add(make_scroll(reg, reg.create(), {{magic::teleport{}}}));
		player_inv.add(make_scroll(reg, reg.create(), {{magic::telescope{}}}));
		player_inv.add(make_quarterstaff(reg, reg.create()));
		player_inv.add(make_bow(reg, reg.create()));

		{ // Add a quiver of arrows.
			inventory quiver_inventory;
			constexpr int arrow_count = 20;
			for (int i = 0; i < arrow_count; ++i) {
				player_inv.add(make_arrow(reg, reg.create()));
			}
			player_inv.add(make_quiver(reg, reg.create(), std::move(quiver_inventory)));
		}

		// Add a gatestone.
		player_inv.add(make_gatestone(reg, reg.create(), magic::color::green, {100_mp, 100_mp}, {0_tick, 10_tick}));

		return player_id;
	}
}
