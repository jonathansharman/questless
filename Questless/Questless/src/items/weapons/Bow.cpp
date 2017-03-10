/**
* @file    Bow.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "items/weapons/Bow.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Action::Complete Bow::BeginFireArrow::perform(Being& actor, cont_t cont)
	{
		/// @todo Check for arrow.

		Bow& bow = _bow;
		return actor.agent().query_tile(std::make_unique<TileQueryRangedTarget>(range), actor.coords, tile_in_range_predicate(actor, range),
			[&actor, cont, &bow](boost::optional<RegionTileCoords> opt_coords) { /// @todo Capturing weapon by reference here is unsafe due to the subsequent delay (actor may have been disarmed, etc.).
				if (opt_coords) {
					double delay = bow.active_cooldown + bow.wind_up();
					actor.add_delayed_action(delay, std::move(cont), CompleteFireArrow::make(bow, *opt_coords));
					return cont(Result::success);
				} else {
					return cont(Result::aborted);
				}
			}
		);
	}

	Action::Complete Bow::CompleteFireArrow::perform(Being& actor, cont_t cont)
	{
		/// @todo Check for arrow again and then spend it.

		actor.busy_time += _bow.follow_through();
		_bow.active_cooldown = _bow.cooldown();
		if (Being* target = actor.region->being(_coords)) {
			Damage damage = _bow.damage();
			_bow.integrity -= _bow.wear_ratio() * damage.total();
			target->take_damage(damage, nullptr, actor.id); /// @todo Part targeting
			return cont(Result::success);
		} else {
			return actor.agent().send_message(std::make_unique<MessageArrowMiss>(), [cont] { return cont(Result::success); });
		}
	}
}
