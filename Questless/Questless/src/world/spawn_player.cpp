//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
	auto create_and_spawn_player(id player_id, id region_id, hud* hud) -> void {
		location location = reg.get<region>(region_id).get_spawn_location();
		id player_id = make_human(reg.create(), location, {player{player_id, hud}});

		// Fill inventory with starting items.
		auto& player_inv = reg.get<inventory>(player_id);

		// Gives the player some magic scrolls.
		player_inv.add(make_scroll(reg.create(), {{magic::shock{}}}));
		player_inv.add(make_scroll(reg.create(), {{magic::heal{}}}));
		player_inv.add(make_scroll(reg.create(), {{magic::teleport{}}}));
		player_inv.add(make_scroll(reg.create(), {{magic::telescope{}}}));
		player_inv.add(make_quarterstaff(reg.create()));
		player_inv.add(make_bow(reg.create()));

		{ // Add a quiver of arrows.
			inventory quiver_inventory;
			constexpr int arrow_count = 20;
			for (int i = 0; i < arrow_count; ++i) {
				player_inv.add(make_arrow(reg.create()));
			}
			player_inv.add(make_quiver(reg.create(), std::move(quiver_inventory)));
		}

		// Add a gatestone.
		player_inv.add(make_gatestone(reg.create(), magic::color::green, {100_mp, 100_mp}, {0_tick, 10_tick}));

		return player_id;
	}
}
