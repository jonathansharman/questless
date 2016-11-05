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
	Action::Complete Weapon::BeginMeleeAttack::perform(Being& actor, cont_t cont)
	{
		Weapon& weapon = _weapon;
		return actor.game().query_tile("Melee Attack", "Choose attack target.", tile_in_range_predicate(actor, 1),
			[&actor, cont, &weapon](boost::optional<RegionTileCoords> opt_coords) { /// @todo Capturing actor and weapon by reference here is unsafe.
				if (opt_coords) {
					double delay = weapon.active_cooldown() + weapon.wind_up();
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
		actor.gain_busy_time(_weapon.follow_through());
		_weapon.active_cooldown(_weapon.cooldown());
		if (Being* target = actor.region().being(_coords)) {
			Damage damage = _weapon.damage();
			_weapon.wear(_weapon.wear_ratio() * damage.total());
			target->take_damage(damage, boost::none, actor.id()); /// @todo Part targeting
			return cont(Result::success);
		} else {
			return actor.agent().message("Melee Attack", "Miss!", [cont] { return cont(Result::aborted); });
		}
	}

	Action::Complete Weapon::Block::perform(Being& /*actor*/, cont_t cont)
	{
		/// @todo This. Blocking also works differently for different weapons. Should there be a default implementation of blocking either?
		return cont(Result::success);
	}
}
