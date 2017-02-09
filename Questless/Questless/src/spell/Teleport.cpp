/**
* @file    TeleportSpell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Teleport spell class.
*/

#include "spell/Teleport.h"
#include "Game.h"
#include "entities/beings/Being.h"

namespace questless::spell
{
	Action::Complete Teleport::perform_cast(Being& caster, Action::cont_t cont)
	{
		return caster.agent().query_tile("Teleport Target", "Select a tile to teleport to.", boost::none, Action::tile_in_range_predicate(caster, _range),
			[this, &caster, cont](boost::optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Action::Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				int distance = caster.coords.distance_to(tile_coords);
				double cost = _cost_factor * distance; /// @todo Reduce cost based on yellow power?
				if (caster.mana < cost) {
					return caster.agent().message("Not enough mana!", "You need " + std::to_string(cost - caster.mana) + " more mana to cast this.", [cont] { return cont(Action::Result::aborted); });
				}
				active_cooldown(cooldown());
				discharge();
				caster.mana -= cost;
				caster.game.add_effect(LightningBoltEffect::make(caster.coords));
				caster.game.add_effect(LightningBoltEffect::make(tile_coords));
				caster.region->move(caster, tile_coords);
				return cont(Action::Result::success);
			}
		);
	}
}
