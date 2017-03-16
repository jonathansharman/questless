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
		bool has_arrow = true;
		if (has_arrow) {
			double delay = _bow.active_cooldown + _bow.wind_up();
			actor.add_delayed_action(delay, std::move(cont), CompleteFireArrow::make(_bow.id));
			return Action::Complete{};
		} else {
			return actor.agent().send_message(std::make_unique<MessageOutOfAmmo>(), [cont] { return cont(Result::aborted); });
		}
	}

	Action::Complete Bow::CompleteFireArrow::perform(Being& actor, cont_t cont)
	{
		if (Bow* bow = dynamic_cast<Bow*>(game().items[_bow_id])) {
			/// @todo Ensure bow is still equipped.
			bool equipped = true;
			if (equipped) {
				/// @todo Check for arrow again.
				bool has_arrow = true;
				if (has_arrow) {
					return actor.agent().query_tile(std::make_unique<TileQueryRangedTarget>(range), actor.coords, tile_in_range_predicate(actor, range),
						[&actor, cont, bow](boost::optional<RegionTileCoords> opt_coords) {
							if (opt_coords) {
								/// @todo Spend arrow.
								actor.busy_time += bow->follow_through();
								bow->active_cooldown = bow->cooldown();
								if (Being* target = actor.region->being(*opt_coords)) {
									Damage damage = bow->damage();
									bow->integrity -= bow->wear_ratio() * damage.total();
									target->take_damage(damage, nullptr, actor.id); /// @todo Part targeting
									return cont(Result::success);
								} else {
									return actor.agent().send_message(std::make_unique<MessageArrowMiss>(), [cont] { return cont(Result::success); });
								}
							} else {
								return cont(Result::aborted);
							}
						}
					);
				} else {
					return actor.agent().send_message(std::make_unique<MessageOutOfAmmo>(), [cont] { return cont(Result::aborted); });
				}
			} else {
				// Bow has been unequipped.
				return cont(Action::Result::aborted);
			}
		} else {
			// Bow has been destroyed.
			return cont(Action::Result::aborted);
		}
	}
}
