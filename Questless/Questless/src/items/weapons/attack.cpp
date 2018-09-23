//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/weapons/attack.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "agents/queries/tile.hpp"
#include "agents/queries/vector.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"
#include "items/weapons/weapon.hpp"
#include "utility/random.hpp"

namespace ql {
	dmg::group attack::damage() const {
		auto& weapon = the_game().items.cref_as<ql::weapon>(weapon_id);
		return base_damage() * (0.5 + weapon.integrity / weapon.durability() / 2.0);
	}

	complete melee_attack::launch::perform(being& actor, cont cont) {
		return _attack->cost().check(actor, [&] {
			return actor.agent().query_vector(queries::vector::melee_attack{}, actor.coords, [](region_tile::vector v) { return v.length() != 0; },
				[&actor, cont, attack = _attack](std::optional<region_tile::vector> opt_vector) {
					if (opt_vector) {
						auto& weapon = the_game().items.cref_as<ql::weapon>(attack->weapon_id);
						double delay = weapon.active_cooldown + attack->wind_up();
						return actor.add_delayed_action(delay, std::move(cont), umake<finish>(attack, *opt_vector));
					} else {
						return cont(result::aborted);
					}
				}
			);
		});
	}

	complete melee_attack::finish::perform(being& actor, cont cont) {
		if (weapon* weapon = the_game().items.ptr_as<ql::weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->opt_bearer_id() == actor.id) {
				_attack->cost().incur(actor);
				actor.busy_time += _attack->follow_through();
				weapon->active_cooldown = _attack->cooldown();
				auto coords = actor.coords + _vector; //! @todo This will need to be more complicated for longer-ranged melee weapons.
				if (being* target = actor.region->being_at(coords)) {
					// Reduce damage based on difference between direction faced and direction attacked.
					constexpr double pct_penalty_per_turn = 0.25;
					dmg::group damage = _attack->damage();
					damage *= 1.0 - pct_penalty_per_turn * region_tile::distance(actor.direction, _vector.direction());

					weapon->integrity -= _attack->wear_ratio() * damage.health_loss();
					{ //! @todo Part targeting. Apply damage to random body part for now.
						auto it = target->body.parts().begin();
						std::advance(it, uniform(std::size_t{0}, target->body.parts().size() - 1));
						target->take_damage(damage, *it, actor.id);
					}
					return cont(result::success);
				} else {
					return actor.agent().send_message(queries::message::melee_miss{}, [cont] { return cont(result::success); });
				}
			} else {
				// Weapon has been unequipped from the actor.
				return cont(action::result::aborted);
			}
		} else {
			// Weapon has been destroyed.
			return cont(action::result::aborted);
		}
	}

	complete ranged_attack::launch::perform(being& actor, cont cont) {
		return _attack->cost().check(actor, [&] {
			weapon const& weapon = the_game().items.cref_as<ql::weapon>(_attack->weapon_id);
			double const delay = weapon.active_cooldown + _attack->wind_up();
			return actor.add_delayed_action(delay, std::move(cont), umake<finish>(_attack));
		});
	}

	complete ranged_attack::finish::perform(being& actor, cont cont) {
		if (weapon* weapon = the_game().items.ptr_as<ql::weapon>(_attack->weapon_id)) {
			if (weapon->equipped() && *weapon->opt_bearer_id() == actor.id) {
				return _attack->cost().check(actor, [&] {
					int const range = _attack->range();
					return actor.agent().query_tile(queries::tile::ranged_attack_target{range}, actor.coords, tile_in_range_predicate(actor, range),
						// Okay to capture weapon by reference; already checked that it's still there, and callback is synchronous here.
						[&actor, cont, attack = _attack, &weapon = *weapon](std::optional<region_tile::point> opt_coords) {
							if (opt_coords) {
								attack->cost().incur(actor);
								actor.busy_time += attack->follow_through();
								weapon.active_cooldown = attack->cooldown();

								// Add attack effect.
								actor.region->add_effect(attack->get_effect(actor.coords, *opt_coords));

								if (being* target = actor.region->being_at(*opt_coords)) {
									return actor.agent().aim_missile(actor.coords, *target, [id = actor.id, cont, &weapon, attack, &target = *target](body_part* body_part) {
										dmg::group damage = attack->damage();
										weapon.integrity -= attack->wear_ratio() * damage.health_loss();
										if (body_part) {
											target.take_damage(damage, *body_part, id);
										}
										return cont(result::success);
									});
								} else {
									return actor.agent().send_message(queries::message::arrow_miss{}, [cont] { return cont(result::success); });
								}
							} else {
								return cont(result::aborted);
							}
						}
					);
				});
			} else {
				// Weapon has been unequipped from the actor.
				return cont(action::result::aborted);
			}
		} else {
			// Weapon has been destroyed.
			return cont(action::result::aborted);
		}
	}
}
