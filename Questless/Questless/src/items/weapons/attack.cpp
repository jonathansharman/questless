//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/attack.h"
#include "items/weapons/weapon.h"
#include "entities/beings/being.h"
#include "game.h"

namespace ql
{
	dmg::group attack::damage() const
	{
		auto& weapon = the_game().items.cref_as<ql::weapon>(weapon_id);
		return base_damage() * (0.5 + weapon.integrity / weapon.durability() / 2.0);
	}

	complete melee_attack::launch::perform(being& actor, cont cont)
	{
		return _attack->cost().check(actor, [&] {
			return actor.agent().query_vector(std::make_unique<vector_query_melee_attack>(), actor.coords, [](region_tile::vector v) { return v.length() != 0; },
				[&actor, cont, attack = _attack](std::optional<region_tile::vector> opt_vector) {
					if (opt_vector) {
						auto& weapon = the_game().items.cref_as<ql::weapon>(attack->weapon_id);
						double delay = weapon.active_cooldown + attack->wind_up();
						return actor.add_delayed_action(delay, std::move(cont), std::make_unique<finish>(attack, *opt_vector));
					} else {
						return cont(result::aborted);
					}
				}
			);
		});
	}

	complete melee_attack::finish::perform(being& actor, cont cont)
	{
		if (weapon* weapon = the_game().items.ptr_as<ql::weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				_attack->cost().incur(actor);
				actor.busy_time += _attack->follow_through();
				weapon->active_cooldown = _attack->cooldown();
				auto coords = actor.coords + _vector; //! @todo This will need to be more complicated for longer-ranged melee weapons.
				if (being* target = actor.region->being_at(coords)) {
					// Reduce damage based on difference between direction faced and direction attacked.
					constexpr double penalty_per_turn = 0.25;
					dmg::group damage = _attack->damage();
					damage *= 1.0 - penalty_per_turn * region_tile::distance(actor.direction, _vector.direction());

					weapon->integrity -= _attack->wear_ratio() * damage.total();
					target->take_damage(damage, nullptr, actor.id); //! @todo Part targeting
					return cont(result::success);
				} else {
					return actor.agent().send_message(std::make_unique<message_melee_miss>(), [cont] { return cont(result::success); });
				}
			} else {
				// weapon.has been unequipped from the actor.
				return cont(action::result::aborted);
			}
		} else {
			// weapon.has been destroyed.
			return cont(action::result::aborted);
		}
	}

	complete ranged_attack::launch::perform(being& actor, cont cont)
	{
		return _attack->cost().check(actor, [&] {
			weapon const& weapon = the_game().items.cref_as<ql::weapon>(_attack->weapon_id);
			double delay = weapon.active_cooldown + _attack->wind_up();
			return actor.add_delayed_action(delay, std::move(cont), std::make_unique<finish>(_attack));
		});
	}

	complete ranged_attack::finish::perform(being& actor, cont cont)
	{
		if (weapon* weapon = the_game().items.ptr_as<ql::weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->bearer_id() == actor.id) {
				return _attack->cost().check(actor, [&] {
					int range = _attack->range();
					return actor.agent().query_tile(std::make_unique<tile_query_ranged_attack_target>(range), actor.coords, tile_in_range_predicate(actor, range),
						// Okay to capture weapon by reference; already checked that it's still there, and callback is synchronous here.
						[&actor, cont, attack = _attack, &weapon = *weapon](std::optional<region_tile::point> opt_coords) {
							if (opt_coords) {
								attack->cost().incur(actor);
								actor.busy_time += attack->follow_through();
								weapon.active_cooldown = attack->cooldown();
								if (being* target = actor.region->being_at(*opt_coords)) {
									dmg::group damage = attack->damage();
									weapon.integrity -= attack->wear_ratio() * damage.total();
									target->take_damage(damage, nullptr, actor.id); //! @todo Part targeting
									return cont(result::success);
								} else {
									return actor.agent().send_message(std::make_unique<message_arrow_miss>(), [cont] { return cont(result::success); });
								}
							} else {
								return cont(result::aborted);
							}
						}
					);
				});
			} else {
				// weapon.has been unequipped from the actor.
				return cont(action::result::aborted);
			}
		} else {
			// weapon.has been destroyed.
			return cont(action::result::aborted);
		}
	}
}
