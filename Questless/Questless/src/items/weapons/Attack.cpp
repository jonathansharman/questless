/**
* @file    Attack.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "items/weapons/Attack.h"
#include "items/weapons/Weapon.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Damage Attack::damage() const
	{
		return base_damage() * (0.5 + weapon.integrity / weapon.durability() / 2.0);
	}

	Action::Complete MeleeAttack::Launch::perform(Being& actor, cont_t cont)
	{
		return actor.agent().query_direction(std::make_unique<DirectionQueryMeleeAttack>(),
			// Okay to capture references since there's no delay before the callback.
			[&actor, cont, &attack = _attack](std::optional<RegionTileCoords::Direction> opt_direction) {
				if (opt_direction) {
					double delay = attack.weapon.active_cooldown + attack.wind_up();
					actor.add_delayed_action(delay, std::move(cont), Finish::make(attack, *opt_direction));
					return cont(Result::success);
				} else {
					return cont(Result::aborted);
				}
			}
		);
	}

	MeleeAttack::Finish::Finish(Attack const& attack, RegionTileCoords::Direction direction)
		: _weapon_id{attack.weapon.id}
		, _name{attack.name()}
		, _damage{attack.damage()}
		, _follow_through{attack.follow_through()}
		, _cooldown{attack.cooldown()}
		, _wear_ratio{attack.wear_ratio()}
		, _direction(direction)
	{}

	Action::Complete MeleeAttack::Finish::perform(Being& actor, cont_t cont)
	{
		if (Weapon* weapon = game().items.get_as<Weapon>(_weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				actor.busy_time += _follow_through;
				weapon->active_cooldown = _cooldown;
				auto coords = actor.coords.neighbor(_direction); /// @todo This will need to be more complicated for longer-ranged melee weapons.
				if (Being* target = actor.region->being(coords)) {
					weapon->integrity -= _wear_ratio * _damage.total();
					target->take_damage(_damage, nullptr, actor.id); /// @todo Part targeting
					return cont(Result::success);
				} else {
					return actor.agent().send_message(std::make_unique<MessageMeleeMiss>(), [cont] { return cont(Result::success); });
				}
			} else {
				// Weapon has been unequipped from the actor.
				return cont(Action::Result::aborted);
			}
		} else {
			// Weapon has been destroyed.
			return cont(Action::Result::aborted);
		}
	}

	Action::Complete RangedAttack::Launch::perform(Being& actor, cont_t cont)
	{
		/// @todo Check for prerequisites, e.g., actor has ammo.
		bool has_prereqs = true;
		if (has_prereqs) {
			double delay = _attack.weapon.active_cooldown + _attack.wind_up();
			actor.add_delayed_action(delay, std::move(cont), Finish::make(_attack));
			return Action::Complete{};
		} else {
			return actor.agent().send_message(std::make_unique<MessageOutOfAmmo>(), [cont] { return cont(Result::aborted); });
		}
	}

	RangedAttack::Finish::Finish(RangedAttack const& attack)
		: _weapon_id{attack.weapon.id}
		, _name{attack.name()}
		, _damage{attack.damage()}
		, _follow_through{attack.follow_through()}
		, _cooldown{attack.cooldown()}
		, _wear_ratio{attack.wear_ratio()}
		, _range{attack.range()}
	{}

	Action::Complete RangedAttack::Finish::perform(Being& actor, cont_t cont)
	{
		if (Weapon* weapon = game().items.get_as<Weapon>(_weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				/// @todo Costs
				bool has_arrow = true;
				if (has_arrow) {
					return actor.agent().query_tile(std::make_unique<TileQueryRangedAttackTarget>(_range), actor.coords, tile_in_range_predicate(actor, _range),
						// Actor and weapon are okay to capture by reference since there's no delay before the callback.
						// Need to capture the finish by value because it will be removed from the delayed actions list and go out of scope just after its perform() call.
						[&actor, cont, weapon, finish = *this](std::optional<RegionTileCoords> opt_coords) mutable {
							if (opt_coords) {
								actor.busy_time += finish._follow_through;
								weapon->active_cooldown = finish._cooldown;
								if (Being* target = actor.region->being(*opt_coords)) {
									weapon->integrity -= finish._wear_ratio * finish._damage.total();
									target->take_damage(finish._damage, nullptr, actor.id); /// @todo Part targeting
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
				// Weapon has been unequipped from the actor.
				return cont(Action::Result::aborted);
			}
		} else {
			// Weapon has been destroyed.
			return cont(Action::Result::aborted);
		}
	}
}
