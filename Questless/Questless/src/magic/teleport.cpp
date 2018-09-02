//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/teleport.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "agents/queries/tile.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "magic/charge_cost.hpp"

namespace ql::magic {
	complete teleport::perform_cast(being& caster, gatestone& gatestone, action::cont cont) {
		return caster.agent().query_tile(queries::tile::teleport_target{}, std::nullopt, action::tile_in_range_predicate(caster, _range),
			[&caster, &gatestone, cont = std::move(cont)](std::optional<region_tile::point> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(action::result::aborted);
				}
				region_tile::point const tile_coords = *opt_tile_coords;
				int const distance = (tile_coords - caster.coords).length();
				return charge_cost{gatestone, _cost_factor * distance}.check_and_incur(caster,
					[&caster, cont = std::move(cont), tile_coords] {
						caster.region->add_effect(smake<lightning_bolt_effect>(caster.coords));
						caster.region->add_effect(smake<lightning_bolt_effect>(tile_coords));
						if (!caster.region->try_move(caster, tile_coords)) {
							//! @todo Try to move to the nearest free location.
						}
						return cont(action::result::success);
					}
				);
			}
		);
	}
}
