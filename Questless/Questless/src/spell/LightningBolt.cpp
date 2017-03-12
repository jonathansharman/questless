/**
* @file    LightningBoltSpell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the LightningBoltSpell class.
*/

#include "spell/LightningBolt.h"
#include "Game.h"
#include "entities/beings/Being.h"
#include "effects/Effect.h"

/// @todo Body part stuff - remove if not used.
#include "entities/beings/BodyPart.h"
#include <deque>
#include <type_traits>

namespace questless::spell
{
	Action::Complete LightningBolt::perform_cast(Being& caster, Action::cont_t cont)
	{
		return caster.agent().query_tile(std::make_unique<TileQueryLightningBoltTarget>(), caster.coords, Action::tile_in_range_predicate(caster, _range),
			[this, &caster, cont](boost::optional<RegionTileCoords> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(Action::Result::aborted);
				}
				RegionTileCoords tile_coords = *opt_tile_coords;
				return caster.agent().query_magnitude(std::make_unique<MagnitudeQueryLightningBolt>(), 20.0, 0.0, boost::none,
					[this, &caster, cont, tile_coords](boost::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(Action::Result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana < cost) {
							return caster.agent().send_message(std::make_unique<MessageNotEnoughMana>(cost - caster.mana), [cont] { return cont(Action::Result::aborted); });
						}
						return caster.agent().get_lightning_bolt_quality(tile_coords, [this, &caster, cont, tile_coords, magnitude, cost](double quality) {
							active_cooldown(cooldown());
							discharge();
							caster.mana -= cost;
							game().add_effect(LightningBoltEffect::make(tile_coords));
							if (Being* target = caster.region->being(tile_coords)) {
								double burn_magnitude = magnitude * quality * caster.stats.magic.yellow / target->stats.antimagic.yellow;

								/// @todo Experimental body part stuff here... Delete or fix.

								// Pick a random strike path from the root to a leaf part.
								BodyPart* part = &target->body.root();
								std::vector<BodyPart*> struck_parts{part};
								while (!part->children().empty()) {
									part = part->children()[uniform(size_t{0}, part->children().size() - 1)].get();
									struck_parts.push_back(part);
								}
								for (auto struck_part : struck_parts) {
									auto burn = Damage{Burn{burn_magnitude * uniform(0.5, 1.5) * part->vitality() / (6.0 * struck_parts.size())}};
									struck_part->take_damage(burn, caster.id);
								}
							}
							return cont(Action::Result::success);
						});
					}
				);
			}
		);
	}
}
