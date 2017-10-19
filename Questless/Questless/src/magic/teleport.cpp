//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/teleport.hpp"
#include "game.hpp"
#include "entities/beings/being.hpp"

namespace ql::magic
{
	complete teleport::perform_cast(being& caster, action::cont cont)
	{
		return caster.agent().query_tile(std::make_unique<tile_query_teleport_target>(), std::nullopt, action::tile_in_range_predicate(caster, _range),
			[this, &caster, cont](std::optional<region_tile::point> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(action::result::aborted);
				}
				region_tile::point tile_coords = *opt_tile_coords;
				int distance = (tile_coords - caster.coords).length();
				double cost = _cost_factor * distance; //! @todo Reduce cost based on yellow power?
				if (caster.mana < cost) {
					return caster.agent().send_message(std::make_unique<message_not_enough_mana>(cost - caster.mana), [cont] { return cont(action::result::aborted); });
				}
				caster.mana -= cost;
				the_game().add_effect(std::make_shared<lightning_bolt_effect>(caster.coords));
				the_game().add_effect(std::make_shared<lightning_bolt_effect>(tile_coords));
				caster.region->move(caster, tile_coords);
				return cont(action::result::success);
			}
		);
	}
}
