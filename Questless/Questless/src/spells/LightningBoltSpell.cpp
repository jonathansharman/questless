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
	void LightningBoltSpell::perform(Being& caster, cont_t cont)
	{
		if (active_cooldown() > 0.0) {
			return caster.agent().message("This spell is on cooldown.", "This spell will be ready in " + std::to_string(active_cooldown()) + ".", [cont] { cont(Result::aborted); });
		}
		if (charges() <= 0) {
			return caster.agent().message("Out of charges!", "You need incant this spell first.", [cont] { cont(Result::aborted); });
		}
		caster.agent().query_tile("Lightning Bolt Target", "Select a tile to be zapped with a lightning bolt.", tile_in_range_predicate(caster, _range),
			[this, &caster, cont](optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				caster.agent().query_magnitude("Lightning Bolt Strength", "Choose how strong to make the lightning bolt.", 20.0, [](double amount) { return amount >= 0.0; },
					[this, &caster, cont, tile_coords](optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana() < cost) {
							return caster.agent().message("Not enough mana!", "You need " + std::to_string(cost - caster.mana()) + " more mana to cast this.", [cont] { cont(Result::aborted); });
						}
						active_cooldown(cooldown());
						discharge();
						caster.lose_mana(cost);
						if (Being* target = caster.region().being(tile_coords)) {
							double burn_magnitude = magnitude * caster.power(color()) / target->resistance(color());

							/// @todo Experimental body part stuff here... Delete or fix.

							// Pick a random path from the root to a leaf part.
							BodyPart* part = &target->body().root();
							std::vector<BodyPart*> struck_parts{part};
							while (!part->children().empty()) {
								part = part->children()[uniform(0u, part->children().size() - 1)].get();
								struck_parts.push_back(part);
							}
							for (auto struck_part : struck_parts) {
								auto burn = Damage::from_burn(burn_magnitude * uniform(0.5, 1.5) * part->vitality() / (6.0 * struck_parts.size()));
								struck_part->take_damage(burn, caster.id());
							}
						}
						return cont(Result::success);
					}
				);
			}
		);
	}
}
