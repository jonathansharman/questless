/**
* @file    Weapon.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "items/weapons/Weapon.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Action::Complete Weapon::BeginMeleeAttack::perform(Being& actor, cont_t cont)
	{
		Weapon& weapon = _weapon;
		return actor.agent().query_tile(std::make_unique<TileQueryMeleeTarget>(), actor.coords, tile_in_range_predicate(actor, 1),
			[&actor, cont, &weapon](boost::optional<RegionTileCoords> opt_coords) { /// @todo Capturing weapon by reference here is unsafe due to the subsequent delay (actor may have been disarmed, etc.).
				if (opt_coords) {
					double delay = weapon.active_cooldown + weapon.wind_up();
					actor.add_delayed_action(delay, std::move(cont), CompleteMeleeAttack::make(weapon, *opt_coords));
					return cont(Result::success);
				} else {
					return cont(Result::aborted);
				}
			}
		);
	}

	Action::Complete Weapon::CompleteMeleeAttack::perform(Being& actor, cont_t cont)
	{
		actor.busy_time += _weapon.follow_through();
		_weapon.active_cooldown = _weapon.cooldown();
		if (Being* target = actor.region->being(_coords)) {
			Damage damage = _weapon.damage();
			_weapon.integrity -= _weapon.wear_ratio() * damage.total();
			target->take_damage(damage, nullptr, actor.id); /// @todo Part targeting
			return cont(Result::success);
		} else {
			return actor.agent().send_message(std::make_unique<MessageMeleeMiss>(), [cont] { return cont(Result::success); });
		}
	}

	Action::Complete Weapon::Block::perform(Being& /*actor*/, cont_t cont)
	{
		/// @todo This. Blocking also works differently for different weapons. Should there be a default implementation of blocking?
		return cont(Result::success);
	}
}
