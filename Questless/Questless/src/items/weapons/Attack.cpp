//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/Attack.h"
#include "items/weapons/Weapon.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Damage Attack::damage() const
	{
		auto& weapon = game().items.get_ref_as<Weapon>(weapon_id);
		return base_damage() * (0.5 + weapon.integrity / weapon.durability() / 2.0);
	}

	Complete MeleeAttack::Launch::perform(Being& actor, cont_t cont)
	{
		return _attack->cost().check(actor, [&] {
			return actor.agent().query_direction(std::make_unique<DirectionQueryMeleeAttack>(),
				[&actor, cont, attack = _attack](std::optional<RegionTileCoords::Direction> opt_direction) {
					if (opt_direction) {
						auto& weapon = game().items.get_ref_as<Weapon>(attack->weapon_id);
						double delay = weapon.active_cooldown + attack->wind_up();
						actor.add_delayed_action(delay, std::move(cont), std::make_unique<Finish>(attack, *opt_direction));
						return cont(Result::success);
					} else {
						return cont(Result::aborted);
					}
				}
			);
		});
	}

	Complete MeleeAttack::Finish::perform(Being& actor, cont_t cont)
	{
		if (Weapon* weapon = game().items.get_as<Weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				_attack->cost().incur(actor);
				actor.busy_time += _attack->follow_through();
				weapon->active_cooldown = _attack->cooldown();
				auto coords = actor.coords.neighbor(_direction); //! @todo This will need to be more complicated for longer-ranged melee weapons.
				if (Being* target = actor.region->being(coords)) {
					// Reduce damage based on difference between direction faced and direction attacked.
					constexpr double penalty_per_turn = 0.25;
					Damage damage = _attack->damage();
					damage *= 1.0 - penalty_per_turn * RegionTileCoords::distance(actor.direction, _direction);

					weapon->integrity -= _attack->wear_ratio() * damage.total();
					target->take_damage(damage, nullptr, actor.id); //! @todo Part targeting
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

	Complete RangedAttack::Launch::perform(Being& actor, cont_t cont)
	{
		return _attack->cost().check(actor, [&] {
			Weapon& weapon = game().items.get_ref_as<Weapon>(_attack->weapon_id);
			double delay = weapon.active_cooldown + _attack->wind_up();
			actor.add_delayed_action(delay, std::move(cont), std::make_unique<Finish>(_attack));
			return Complete{};
		});
	}

	Complete RangedAttack::Finish::perform(Being& actor, cont_t cont)
	{
		if (Weapon* weapon = game().items.get_as<Weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				return _attack->cost().check(actor, [&] {
					int range = _attack->range();
					return actor.agent().query_tile(std::make_unique<TileQueryRangedAttackTarget>(range), actor.coords, tile_in_range_predicate(actor, range),
						// Okay to capture weapon by reference; already checked that it's still there, and callback is synchronous here.
						[&actor, cont, attack = _attack, &weapon = *weapon](std::optional<RegionTileCoords> opt_coords) {
							if (opt_coords) {
								attack->cost().incur(actor);
								actor.busy_time += attack->follow_through();
								weapon.active_cooldown = attack->cooldown();
								if (Being* target = actor.region->being(*opt_coords)) {
									Damage damage = attack->damage();
									weapon.integrity -= attack->wear_ratio() * damage.total();
									target->take_damage(damage, nullptr, actor.id); //! @todo Part targeting
									return cont(Result::success);
								} else {
									return actor.agent().send_message(std::make_unique<MessageArrowMiss>(), [cont] { return cont(Result::success); });
								}
							} else {
								return cont(Result::aborted);
							}
						}
					);
				});
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
