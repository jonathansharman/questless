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

		Bow& bow = _bow; // Safe to capture bow here because callback is immediate.
		return actor.agent().query_tile(std::make_unique<TileQueryRangedTarget>(range), actor.coords, tile_in_range_predicate(actor, range),
			[&actor, cont, &bow](boost::optional<RegionTileCoords> opt_coords) {
				if (opt_coords) {
					double delay = bow.active_cooldown + bow.wind_up();
					actor.add_delayed_action(delay, std::move(cont), CompleteFireArrow::make(bow.id, *opt_coords));
					return cont(Result::success);
				} else {
					return cont(Result::aborted);
				}
			}
		);
	}

	Action::Complete Bow::CompleteFireArrow::perform(Being& actor, cont_t cont)
	{
		if (Bow* bow = dynamic_cast<Bow*>(game().items[_bow_id])) {
			/// @todo Ensure bow is still equipped.
			bool equipped = true;
			if (equipped) {
				/// @todo Check for arrow again and then spend it.
				actor.busy_time += bow->follow_through();
				bow->active_cooldown = bow->cooldown();
				if (Being* target = actor.region->being(_coords)) {
					Damage damage = bow->damage();
					bow->integrity -= bow->wear_ratio() * damage.total();
					target->take_damage(damage, nullptr, actor.id); /// @todo Part targeting
					return cont(Result::success);
				} else {
					return actor.agent().send_message(std::make_unique<MessageArrowMiss>(), [cont] { return cont(Result::success); });
				}
			}
		}
		// Bow has been removed.
		return cont(Action::Result::aborted);
	}
}
