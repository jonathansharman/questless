//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agent.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"
#include "items/inventory.hpp"
#include "magic/spell.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"
#include "world/tile.hpp"

namespace ql {
	action::result agent::idle(tick duration) {
		reg.get<conditions>(id).busy_time += duration;
		return action::result::success;
	}

	action::result agent::turn(region_tile::direction direction) {
		auto [stats, cond] = reg.get<stats::being, conditions>(id);
		if (stats.a.agility > 0.0_agi) {
			// Base cost of turning any amount.
			constexpr auto base_delay = 1_tick;

			// Cost per turning distance from current direction to new direction.
			constexpr auto delay_per_turn = 1_tick;

			auto const agility_factor = 100.0_agi / stats.a.agility.value();
			auto const turn_delay = delay_per_turn * region_tile::distance(cond.direction, direction);
			cond.busy_time += cancel::quantity_cast<tick>(agility_factor * (base_delay + turn_delay));
			cond.direction = direction;

			return action::result::success;
		} else {
			return action::result::failure;
		}
	}

	action::result agent::walk(region_tile::direction direction) {
		auto [stats, location] = reg.get<ql::stats::being, ql::location>(id);

		if (stats.a.agility <= 0.0_agi) { return action::result::failure; }

		// Base cost of moving.
		constexpr auto base_delay = 1_tick;

		// Cost per turning distance from direction faced to direction moved.
		constexpr auto delay_per_turn = 1_tick;

		auto const agility_factor = 100.0_agi / stats.a.agility.value();
		auto& region = reg.get<ql::region>(location.region_id);

		// Move the being.
		if (!region.try_move(id, location.coords.neighbor(direction))) { return action::result::failure; }

		// Increase busy time.
		auto& cond = reg.get<conditions>(id);
		auto const strafe_delay = delay_per_turn * region_tile::distance(cond.direction, direction);
		//! @todo Account for terrain.
		cond.busy_time += cancel::quantity_cast<tick>(agility_factor * (base_delay + strafe_delay));

		return action::result::success;
	}

	action::result agent::fly() {
		//! @todo This.
		return action::result::failure;
	}

	action::result agent::drop(ent item_id) {
		auto& inv = reg.get<inventory>(id);
		if (auto it = inv.item_ids.find(item_id) != inv.item_ids.end()) {
			// Remove item from inventory.
			inv.item_ids.erase(it);
			// Get region.
			auto const& location = reg.get<ql::location>(id);
			auto& region = reg.get<ql::region>(location.region_id);
			// The tile certainly exists because a being is located on it, so it's safe to dereference without a check here.
			ent tile_id = *region.tile_at(location.coords);
			// Add item to the tile's inventory.
			reg.get<inventory>(tile_id).add(item_id);

			return action::result::success;
		} else {
			return action::result::failure;
		}
	}

	action::result agent::toss(ent item_id) {
		//! @todo This.
		return drop(item_id);
	}
}
