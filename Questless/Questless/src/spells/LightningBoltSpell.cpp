/**
* @file    LightningBoltSpell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the LightningBoltSpell class.
*/

#include "spells/LightningBoltSpell.h"
#include "Game.h"
#include "entities/beings/Being.h"

/// @todo Body part stuff - remove if not used.
#include "entities/beings/BodyPart.h"
#include <deque>
#include <type_traits>

namespace questless
{
	void LightningBoltSpell::perform(Game& game, Being& caster, cont_t cont)
	{
		if (active_cooldown() > 0.0) {
			return caster.agent().message(game, "This spell is on cooldown.", "This spell will be ready in " + std::to_string(active_cooldown()) + ".", [cont] { cont(Result::aborted); });
		}
		if (charges() <= 0) {
			return caster.agent().message(game, "Out of charges!", "You need incant this spell first.", [cont] { cont(Result::aborted); });
		}
		caster.agent().query_tile(game, "Lightning Bolt Target", "Select a tile to be zapped with a lightning bolt.", tile_in_range_predicate(caster, _range),
			[this, &game, &caster, cont](optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				caster.agent().query_magnitude(game, "Lightning Bolt Strength", "Choose how strong to make the lightning bolt.", 20.0, [](double amount) { return amount >= 0.0; },
					[this, &game, &caster, cont, tile_coords](optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana() < cost) {
							return caster.agent().message(game, "Not enough mana!", "You need " + std::to_string(cost - caster.mana()) + " more mana to cast this.", [cont] { cont(Result::aborted); });
						}
						active_cooldown(cooldown());
						discharge();
						caster.lose_mana(cost);
						for (Being& target : game.region().beings(tile_coords)) {
							double burn_magnitude = magnitude * caster.power(color()) / target.resistance(color());
							auto burn = Damage::from_burn(burn_magnitude * 10000000000); /// @todo Debug damage... Remove.

							/// @todo Experimental body part stuff here... Delete or fix.
							double average_damage_per_part = burn_magnitude / target.body().parts_count();
							for (auto part : target.body().parts()) {
								part->take_damage(uniform(0.0, 2.0 * average_damage_per_part));
							}

							target.take_damage(burn, &caster);
						}
						return cont(Result::success);
					}
				);
			}
		);
	}
}
