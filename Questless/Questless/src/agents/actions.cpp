//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "actions.hpp"

#include "entities/beings/body.hpp"
#include "entities/beings/body_cond.hpp"
#include "entities/beings/stats/body.hpp"
#include "items/inventory.hpp"
#include "world/region.hpp"

namespace ql {
	void turn(ent turner_id, region_tile::direction direction) {
		auto& body = reg.get<ql::body>(turner_id);

		constexpr auto base_cost = 1_ap;
		constexpr auto cost_per_turn = 1_ap;
		auto const turn_cost = base_cost + cost_per_turn * region_tile::distance(body.cond.direction, direction);

		//! @todo Spend leg ability points.

		body.cond.direction = direction;
	}

	void walk(ent walker_id, region_tile::direction direction) {
		auto [body, location] = reg.get<ql::body, ql::location>(walker_id);

		constexpr auto base_cost = 1_ap;
		constexpr auto cost_per_turn = 1_tick;

		auto& region = reg.get<ql::region>(location.region_id);

		//! @todo Spend leg ability points.
		// cond.busy_time += cancel::quantity_cast<tick>(agility_factor * (base_delay + strafe_delay));

		//! @todo Account for terrain.

		// Move the being.
		if (!region.try_move(walker_id, location.coords.neighbor(direction))) { return; }

		// Increase busy time.
		auto const strafe_cost = cost_per_turn * region_tile::distance(body.cond.direction, direction);
	}

	void fly(ent /*flyer_id*/) {
		//! @todo This.
	}

	void drop(ent dropper_id, ent item_id) {
		// Get dropper's inventory.
		auto& inv = reg.get<inventory>(dropper_id);
		// Find item in inventory.
		auto const it = inv.item_ids.find(item_id);
		// Item not present in dropper's inventory.
		if (it == inv.item_ids.end()) { return; }
		// Remove item from inventory.
		inv.item_ids.erase(it);
		// Get region.
		auto const& location = reg.get<ql::location>(dropper_id);
		auto& region = reg.get<ql::region>(location.region_id);
		// The tile certainly exists because a being is located on it, so it's safe to dereference without a check here.
		ent tile_id = *region.tile_at(location.coords);
		// Add item to the tile's inventory.
		reg.get<inventory>(tile_id).add(item_id);
	}

	void toss(ent thrower_id, ent item_id) {
		//! @todo This.
		return drop(thrower_id, item_id);
	}
}
