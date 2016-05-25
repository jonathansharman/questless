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
		caster.agent().query_tile(game, "Lightning Bolt Target", "Select a tile to be zapped with a lightning bolt.", tile_in_range_predicate(caster, LightningBoltSpellK::range),
			[this, &game, &caster, cont](optional<HexCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Result::aborted);
				}
				HexCoords tile_coords = *opt_tile_coords;
				caster.agent().query_magnitude(game, "Lightning Bolt Strength", "Choose how strong to make the lightning bolt.", 20.0, [](double amount) { return amount >= 0.0; },
					[this, &game, &caster, cont, tile_coords](optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = LightningBoltSpellK::cost_factor * magnitude * log2(magnitude + k());
						if (caster.mana() < cost) {
							return caster.agent().message(game, "Not enough mana!", "You need " + std::to_string(cost - caster.mana()) + " more mana to cast this.", [cont] { cont(Result::aborted); });
						}
						active_cooldown(cooldown());
						discharge();
						caster.lose_mana(cost);
						if (auto target = dynamic_cast<Being*>(game.region().entity(tile_coords))) {
							auto burn = Damage::from_burn(magnitude * caster.power(color()) / target->resistance(color()));
							target->take_damage(burn, &caster);
						}
						return cont(Result::success);
					}
				);
			}
		);
	}
}
