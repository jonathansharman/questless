//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/shock.h"

#include "game.h"
#include "entities/beings/being.h"
#include "effects/effect.h"
#include "magic/mana_cost.h"

//! @todo Body part stuff - remove if not used.
#include "entities/beings/body_part.h"
#include <deque>
#include <type_traits>

namespace ql::magic
{
	complete shock::perform_cast(being& caster, action::cont cont)
	{
		return caster.agent().query_tile(std::make_unique<tile_query_shock_target>(), caster.coords, action::tile_in_range_predicate(caster, _range),
			[this, &caster, cont](std::optional<region_tile::point> opt_tile_coords) {
				if (!opt_tile_coords) {
					return cont(action::result::aborted);
				}
				region_tile::point tile_coords = *opt_tile_coords;
				return caster.agent().query_magnitude(std::make_unique<magnitude_query_shock>(), 20.0, 0.0, std::nullopt,
					[this, &caster, cont, tile_coords](std::optional<double> opt_magnitude) {
						if (!opt_magnitude) {
							return cont(action::result::aborted);
						}
						double magnitude = *opt_magnitude;
						double cost = _cost_factor * magnitude * log2(magnitude + _cost_log);
						if (caster.mana < cost) {
							return caster.agent().send_message
								( std::make_unique<message_not_enough_mana>(cost - caster.mana)
								, [cont] { return cont(action::result::aborted); }
								);
						}
						return caster.agent().get_shock_quality(tile_coords, [this, &caster, cont, tile_coords, magnitude, cost](double quality) {
							caster.mana -= cost;
							the_game().add_effect(std::make_shared<lightning_bolt_effect>(tile_coords));
							if (being* target = caster.region->being_at(tile_coords)) {
								double shock_magnitude = magnitude * quality;

								//! @todo Experimental body part stuff here... Delete or fix.

								// Pick a random strike path from the root to a leaf part.
								body_part* part = &target->body.root();
								std::vector<body_part*> struck_parts{part};
								while (!part->children().empty()) {
									part = part->children()[uniform(std::size_t{0}, part->children().size() - 1)].get();
									struck_parts.push_back(part);
								}
								for (auto struck_part : struck_parts) {
									dmg::group shock = dmg::shock{shock_magnitude * uniform(0.5, 1.5) * part->vitality() / (4.0 * struck_parts.size())};
									struck_part->take_damage(shock, caster.id);
								}
							}
							return cont(action::result::success);
						});
					}
				);
			}
		);
	}
}
