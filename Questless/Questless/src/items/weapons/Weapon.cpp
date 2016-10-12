/**
* @file    Weapon.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Weapon abstract base class.
*/

#include "items/weapons/Weapon.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Action::Complete Weapon::Ready::perform(Being& actor, cont_t cont)
	{
		actor.gain_busy_time(_weapon.active_cooldown() + _weapon.wind_up());
		_weapon._ready = true;
		return cont(Result::success);
	}

	Action::Complete Weapon::MeleeAttack::perform(Being& actor, cont_t cont)
	{
		Weapon& weapon = _weapon;
		return actor.game().query_tile("Melee Attack", "Choose attack target.", tile_in_range_predicate(actor, 1),
			[&actor, cont, &weapon](boost::optional<RegionTileCoords> opt_coords) {
				if (opt_coords) {
					if (Being* target = actor.region().being(*opt_coords)) {
						actor.gain_busy_time(weapon.follow_through());
						weapon.active_cooldown(weapon.cooldown());
						weapon._ready = false;
						Damage damage = weapon.damage();
						weapon.wear(weapon.wear_ratio() * damage.total());
						target->take_damage(damage, boost::none, actor.id()); /// @todo Part targeting
						return cont(Result::success);
					}
				}
				return cont(Result::aborted);
			}
		);
	}

	Action::Complete Weapon::Block::perform(Being& /*actor*/, cont_t cont)
	{
		/// @todo This. Blocking also works differently for different weapons. Should there be a default implementation of blocking either?
		return cont(Result::success);
	}
}
