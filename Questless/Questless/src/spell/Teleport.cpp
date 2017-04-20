//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell/Teleport.h"
#include "Game.h"
#include "entities/beings/Being.h"

namespace questless::spell
{
	Complete Teleport::perform_cast(Being& caster, Action::cont_t cont)
	{
		return caster.agent().query_tile(std::make_unique<TileQueryTeleportTarget>(), std::nullopt, Action::tile_in_range_predicate(caster, _range),
			[this, &caster, cont](std::optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Action::Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				int distance = caster.coords.distance_to(tile_coords);
				double cost = _cost_factor * distance; //! @todo Reduce cost based on yellow power?
				if (caster.mana < cost) {
					return caster.agent().send_message(std::make_unique<MessageNotEnoughMana>(cost - caster.mana), [cont] { return cont(Action::Result::aborted); });
				}
				active_cooldown(cooldown());
				discharge();
				caster.mana -= cost;
				game().add_effect(std::make_shared<LightningBoltEffect>(caster.coords));
				game().add_effect(std::make_shared<LightningBoltEffect>(tile_coords));
				caster.region->move(caster, tile_coords);
				return cont(Action::Result::success);
			}
		);
	}
}
