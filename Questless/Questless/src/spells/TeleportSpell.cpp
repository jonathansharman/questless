/**
* @file    TeleportSpell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the TeleportSpell class.
*/

#include "spells/TeleportSpell.h"
#include "Game.h"
#include "entities/beings/Being.h"

namespace questless
{
	void TeleportSpell::perform(Being& caster, cont_t cont)
	{
		if (active_cooldown() > 0.0) {
			return caster.agent().message("This spell is on cooldown.", "This spell will be ready in " + std::to_string(active_cooldown()) + ".", [cont] { cont(Result::aborted); });
		}
		if (charges() <= 0) {
			return caster.agent().message("Out of charges!", "You need incant this spell first.", [cont] { cont(Result::aborted); });
		}
		caster.agent().query_tile("Teleport Target", "Select a tile to teleport to.", tile_in_range_predicate(caster, _range),
			[this, &caster, cont](optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				int distance = caster.coords().hex.distance_to(tile_coords.hex);
				double cost = _cost_factor * distance;
				if (caster.mana() < cost) {
					return caster.agent().message("Not enough mana!", "You need " + std::to_string(cost - caster.mana()) + " more mana to cast this.", [cont] { cont(Result::aborted); });
				}
				active_cooldown(cooldown());
				discharge();
				caster.lose_mana(cost);
				caster.region().move(caster, tile_coords);
				return cont(Result::success);
			}
		);
	}
}
